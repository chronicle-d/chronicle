from flask import Flask
from flask_cors import CORS
from calls import register_all_blueprints
from chronicle import ChronicleDB, ChronicleException, getErrorMsg
import sys

def create_app():
    try:
        ChronicleDB().connect()
        ChronicleDB().initDB()
    except (ChronicleException, Exception) as e:
        if isinstance(e, ChronicleException):
            errorMsg = getErrorMsg(e.code)
            if (e.code == 10001):
                sys.stderr.write(f"Error {e.code}, {errorMsg} => Make sure that MongoD is up. ({e.details})\n")
            else:
                sys.stderr.write(f"Error {e.code}, {errorMsg} => {e.details}\n")
        else:
            sys.stderr.write(f"Pythonic error: {e}\n")
        exit(1)

    app = Flask(__name__)
    register_all_blueprints(app)
    return app

app = create_app()
CORS(app)

if __name__ == "__main__":
    app.run(debug=True)
