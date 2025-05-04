from flask import Flask, request, render_template
import joblib

app = Flask(__name__)
model = joblib.load("model.pkl")
model, vectorizer = joblib.load("model.pkl")


@app.route("/", methods=["GET", "POST"])
def index():
    prediction = ""
    if request.method == "POST":
        text = request.form["tweet"]
        vect = vectorizer.transform([text])
        pred = model.predict(vect)[0]
        prediction = "Positive" if pred == 1 else "Negative"
    return render_template("index.html", prediction=prediction)

if __name__ == "__main__":
    app.run(debug=True)