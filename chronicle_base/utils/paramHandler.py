from core.response import makeResponse

# This file contains helpers to handle paramters in API calls
def validateParams(params: dict, schema: dict):
    validated = {}

    for key, rule in schema.items():
        value = params.get(key)

        if rule.get("required") and (value is None or value.strip() == ""):
            return makeResponse(False, rule["required_error"], {}, 400), None

        if value is None:
            if rule.get("type") is str:
                validated[key] = ""
            continue

        if "type" in rule:
            try:
                value = rule["type"](value)
            except (ValueError, TypeError):
                return makeResponse(False, rule.get("value_error", f"{key} has invalid type"), {}, 400), None

        if "allowed" in rule and value not in rule["allowed"]:
            return makeResponse(False, rule["value_error"], {}, 400), None

        if "range" in rule:
            min_val, max_val = rule["range"]
            if not (min_val <= value <= max_val):
                return makeResponse(False, rule["value_error"], {}, 400), None

        validated[key] = value

    return None, validated
