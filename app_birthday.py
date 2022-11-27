import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # TODO: Add the user's entry into the database
        name = request.form.get("name") #get the value entered by the user in the form on the page and store it to a variable, to be used in sqlite3
        month = request.form.get("month")# variable name = request.form.get("name of the input type")
        day = request.form.get("day")

        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day) #this is the sql query to insert a data to a database
                                                                                            #VALUES (?), variable name = is the syntax for placeholder data

        return redirect("/")

    else: #if request method is get

        # TODO: Display the entries in the database on index.html
        birthdays = db.execute("SELECT * FROM birthdays") #this is how to query on a database using flask/python, saving all of that rows inside a list variable (birthdays)

        return render_template("index.html", birthdays = birthdays) #need to pass the list to the template


