import sqlite3
import inspect

# === In-Memory Database Connection ===
conn = sqlite3.connect(":memory:")
cursor = conn.cursor()

# === ORM Base ===
class Model:
    def __init_subclass__(cls):
        cls._fields = {
            k: v for k, v in cls.__annotations__.items() if k != "id"
        }
        cls._table = cls.__name__.lower()
        fields_sql = ", ".join(f"{k} {Model.map_type(v)}" for k, v in cls._fields.items())
        sql = f"CREATE TABLE IF NOT EXISTS {cls._table} (id INTEGER PRIMARY KEY AUTOINCREMENT, {fields_sql})"
        cursor.execute(sql)

    @staticmethod
    def map_type(py_type):
        return "TEXT" if py_type == str else "INTEGER"

    def save(self):
        values = [getattr(self, field) for field in self._fields]
        placeholders = ", ".join("?" for _ in values)
        sql = f"INSERT INTO {self._table} ({', '.join(self._fields)}) VALUES ({placeholders})"
        cursor.execute(sql, values)
        conn.commit()
        self.id = cursor.lastrowid

    @classmethod
    def all(cls):
        cursor.execute(f"SELECT * FROM {cls._table}")
        rows = cursor.fetchall()
        return [cls._from_row(row) for row in rows]

    @classmethod
    def _from_row(cls, row):
        obj = cls.__new__(cls)
        for i, col in enumerate(['id'] + list(cls._fields)):
            setattr(obj, col, row[i])
        return obj

    @classmethod
    def find(cls, id):
        cursor.execute(f"SELECT * FROM {cls._table} WHERE id = ?", (id,))
        row = cursor.fetchone()
        return cls._from_row(row) if row else None

# === Models with Relationships ===
class User(Model):
    name: str

    def posts(self):
        return [post for post in Post.all() if post.user_id == self.id]

class Post(Model):
    title: str
    user_id: int

    def user(self):
        return User.find(self.user_id)

class Tag(Model):
    name: str

    def posts(self):
        sql = """SELECT post_id FROM post_tags WHERE tag_id = ?"""
        cursor.execute(sql, (self.id,))
        post_ids = [row[0] for row in cursor.fetchall()]
        return [Post.find(pid) for pid in post_ids]

# === Many-to-Many Join Table Manually ===
cursor.execute("CREATE TABLE IF NOT EXISTS post_tags (post_id INTEGER, tag_id INTEGER)")

def link_post_tag(post: Post, tag: Tag):
    cursor.execute("INSERT INTO post_tags (post_id, tag_id) VALUES (?, ?)", (post.id, tag.id))
    conn.commit()

# === Demo ===
if __name__ == "__main__":
    u1 = User()
    u1.name = "Alice"
    u1.save()

    p1 = Post()
    p1.title = "Hello World"
    p1.user_id = u1.id
    p1.save()

    t1 = Tag()
    t1.name = "Intro"
    t1.save()

    link_post_tag(p1, t1)

    # Output
    print("Users:")
    for u in User.all():
        print(f"{u.id}: {u.name}")

    print("\nPosts:")
    for p in Post.all():
        print(f"{p.id}: {p.title} by {p.user().name}")

    print("\nTags for Post 1:")
    for t in t1.posts():
        print(f"Post: {t.title}")
