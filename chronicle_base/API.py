from flask import Flask
from flask_cors import CORS
from calls import register_all_blueprints
from core.chronicle import initChronicleConfig


def create_app():
    initChronicleConfig()
    app = Flask(__name__)
    register_all_blueprints(app)
    return app


app = create_app()
CORS(app)

if __name__ == "__main__":
    app.run(debug=True)
