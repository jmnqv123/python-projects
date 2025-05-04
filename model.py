# model.py
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.naive_bayes import MultinomialNB
import joblib

# Training data
texts = ["I love it", "Horrible experience", "Fantastic!", "Not good", "Very bad", "Awesome work"]
labels = [1, 0, 1, 0, 0, 1]  # 1 = positive, 0 = negative

# Train the model
vectorizer = CountVectorizer()
X = vectorizer.fit_transform(texts)
model = MultinomialNB()
model.fit(X, labels)

# Save model and vectorizer
model, vectorizer = joblib.load("model.pkl")
print("✅ Model saved as model.pkl")
