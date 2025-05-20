from flask import Flask, request, Response
from flask_restful import reqparse, abort, Api, Resource
from flask_cors import CORS

import os
import dotenv

def get_config():
    config_dict = {}
    
    dotenv.load_dotenv()
    config_dict["FLASK_SECRET_KEY"] = os.getenv("FLASK_SECRET_KEY", "default_key")
    config_dict["FRONTEND_URL"] = os.getenv("FRONTEND_URL", "*")

    return config_dict

config = get_config()

app = Flask(__name__)

# Get env variables here
app.secret_key = config.get("FLASK_SECRET_KEY", "mykey")
frontend_url = config.get("FRONTEND_URL", "*")

api = Api(app)
cors = CORS(app, resources={r"/*": {"origins": frontend_url}})