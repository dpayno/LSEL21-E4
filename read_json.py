import json

with open('config.json') as file:
    config_dict = json.load(file)
    
    print(config_dict["PIN"])
    print(config_dict["APN"])