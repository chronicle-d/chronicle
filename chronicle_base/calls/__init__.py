import os
import pkgutil
import importlib
import inspect
from flask import Blueprint

__all__ = []


# Automatically find all Blueprints named `my_blueprint` in this package
def register_all_blueprints(app):
    package_path = os.path.dirname(__file__)
    for _, module_name, _ in pkgutil.iter_modules([package_path]):
        module = importlib.import_module(f"{__name__}.{module_name}")
        for _, obj in inspect.getmembers(module):
            if isinstance(obj, Blueprint):
                app.register_blueprint(obj)

