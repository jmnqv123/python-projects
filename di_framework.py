import inspect
from abc import ABC, abstractmethod
from functools import wraps

# === Dependency Container ===
class Container:
    def __init__(self):
        self._providers = {}
        self._singletons = {}

    def register(self, iface, cls=None, singleton=False):
        if cls is None:
            cls = iface
        self._providers[iface] = (cls, singleton)

    def resolve(self, iface):
        if iface in self._singletons:
            return self._singletons[iface]
        if iface not in self._providers:
            raise Exception(f"No provider registered for {iface}")
        cls, singleton = self._providers[iface]
        params = inspect.signature(cls).parameters
        dependencies = {
            name: self.resolve(param.annotation)
            for name, param in params.items()
            if param.annotation != inspect._empty and param.annotation is not Container
        }
        instance = cls(**dependencies)
        if singleton:
            self._singletons[iface] = instance
        return instance

# === Autowire Decorator (Fixed) ===
def inject(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        container = kwargs.get('container')
        if container is None:
            raise Exception("You must pass 'container=...' when calling the function.")

        sig = inspect.signature(func)
        bound = sig.bind_partial(*args, **kwargs)

        for name, param in sig.parameters.items():
            if name not in bound.arguments and param.annotation != inspect._empty:
                if param.annotation is Container:
                    continue  # Don't autowire container
                bound.arguments[name] = container.resolve(param.annotation)

        bound.arguments['container'] = container  # Pass container explicitly

        return func(*bound.args, **bound.kwargs)
    return wrapper

# === Interfaces ===
class NotificationService(ABC):
    @abstractmethod
    def notify(self, message: str):
        pass

class UserRepository(ABC):
    @abstractmethod
    def save_user(self, name: str):
        pass

# === Implementations ===
class EmailService(NotificationService):
    def notify(self, message: str):
        print(f"[Email] {message}")

class InMemoryUserRepo(UserRepository):
    def __init__(self):
        self.users = []

    def save_user(self, name: str):
        self.users.append(name)
        print(f"User '{name}' saved to memory.")

# === Application Logic ===
class UserService:
    def __init__(self, repo: UserRepository, notifier: NotificationService):
        self.repo = repo
        self.notifier = notifier

    def register_user(self, name: str):
        self.repo.save_user(name)
        self.notifier.notify(f"User {name} registered.")

# === Entry Point ===
@inject
def main(service: UserService, *, container: Container):
    service.register_user("Alice")
    service.register_user("Bob")

# === Setup and Run ===
if __name__ == "__main__":
    container = Container()
    container.register(UserRepository, InMemoryUserRepo, singleton=True)
    container.register(NotificationService, EmailService)
    container.register(UserService)

    main(container=container)

