import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        user_id = session["user_id"]  # get the user id of the current user
        stocks = db.execute(
            "SELECT symbol, name, price, SUM(shares) as totalShares FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)  # create a list
        # select it and group it by symbol so that every transactions with the same symbol will be merged
        # add all shares and find the sum, name the result as totalShares for later use

        usercash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]  # get the cash value of the user
        total = usercash  # we will add the currect cash owned by the user and the value of the stocks they owned to get the total amount they owned

        for stock in stocks:
            # for every stocks the user bought, get the total by multiplying the total shares bought with the current price of the stocks
            total += stock["price"] * stock["totalShares"]

        return render_template("index.html", tables=stocks, cash=usd(usercash), total=usd(total), usd=usd)
        # pass the usd funtion to the html so that it can be used in the jinja template
    else:
        return render_template("addcash.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Must enter symbol")

        shares = request.form.get("shares")
        if not request.form.get("shares"):
            return apology("Must enter shares to buy")

        info = lookup(request.form.get("symbol"))
        if info is None:
            return apology("Invalid Symbol")

        user_id = session["user_id"]

        symbol = info["symbol"]
        stockname = info["name"]
        stockprice = info["price"]

        user = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        cash = user[0]["cash"]

        totalprice = stockprice * int(shares)

        if totalprice > cash:
            return apology("Sorry, Insufficient Cash")
        else:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash - totalprice, user_id)
            db.execute("INSERT INTO transactions (user_id, name, shares, price, action, symbol) VALUES (?, ?, ?, ? ,? ,?)",
                       user_id, stockname, shares, stockprice, 'BUY', symbol)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    stocks = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)

    return render_template("history.html", stocks=stocks, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("Must enter symbol")

        info = lookup(request.form.get("symbol"))

        if info is None:
            return apology("Invalid Symbol")

        symbol = info["symbol"]
        stockname = info["name"]
        stockprice = usd(info["price"])

        # this is the syntax for when inputing a value inside an html file from a flask code
        return render_template("quoted.html", stockname=stockname, symbol=symbol, stockprice=stockprice)
    else:
        return render_template("quote.html")


@app.route("/quoted", methods=["GET", "POST"])
@login_required
def quoted():

    if request.method == "POST":
        return redirect("/quote")
    else:
        return render_template("quoted.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":  # if theres a form being submitted, then the method is POST
        username = request.form.get("username")  # get the input inside the inputfield "username" and store it in a variable
        if not request.form.get("username"):  # if the inputfield is empty, return apology
            return apology("must provide username")

        password = request.form.get("password")  # get the input inside the inputfield "password" and store it in a variable
        if not request.form.get("password"):  # if the inputfield is empty, return apology
            return apology("must provide password")

        # get the input inside the inputfield "confirmation" and store it in a variable
        confirmation = request.form.get("confirmation")
        if not request.form.get("confirmation"):
            # if the inputfield is empty, return apology
            return apology("must confirm password")

        if password != confirmation:  # check if password and confirmation is not equal, return apology
            return apology("password should be the same")

        # execute sqlite query to know if theres a similar username, and store it to a variable
        usernamecheck = db.execute("SELECT * FROM users WHERE username = '" + username + "'")
        if len(usernamecheck) == 1:  # if the row inside the variable is 1 (if theres any value inside the variable), then return apology
            return apology("username already taken")

        # user generate hash to make a hash version of the password and store it in a variable
        pwhash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, pwhash)

        # insert the values username and hashedpassword to the database using sqlite3 queries
        # after all of that, redirect the user to the login page
        return redirect("/login")

    else:
        # if the link is getting clicked and you want to show the hmtl page, then the method is GET, return template the html file to show
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        user_id = session["user_id"]
        stock = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", user_id)

        # if method is get, get all the symbols of stocks on db and give it to sell.html
        return render_template("sell.html", tables=stock)

    else:
        user_id = session["user_id"]

        if not request.form.get("shares"):
            return apology("Must input shares to be sold")
        if not request.form.get("symbol"):
            return apology("Must pick Stock to be sold")
        symbol = request.form.get("symbol")
        info = lookup(symbol)

        shares = request.form.get("shares")
        sharess = int(shares)
        stockprice = info["price"]
        stockname = info["name"]
        num_shares = db.execute(
            "SELECT SUM(shares) as totalShares FROM transactions WHERE user_id = ? AND symbol = ? AND action = 'BUY' GROUP BY symbol", user_id, symbol)[0]["totalShares"]
        # count all the shares that the user bought

        if num_shares < sharess:  # check if entered shares is lower than shares owned
            return apology("Not enough shares owned")

        selling_price = sharess * stockprice  # get the total price the user will get when selling the stock shares
        user_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        updated_cash = user_cash + selling_price

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)  # update the cash owned by the user
        db.execute("INSERT INTO transactions (user_id, name, shares, price, action, symbol) VALUES (?, ?, ?, ? ,? ,?)",
                   user_id, stockname, -sharess, stockprice, 'SELL', symbol)  # create an entry to the transaction table
        # set shares as -shares so that it will subtract instead of add
        # set SELL as action because we will sell a stock

        return redirect("/")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Add cash to account"""
    if request.method == "POST":

        if not request.form.get("addcash"):
            return apology("Must enter amount")
        addcash = request.form.get("addcash")
        user_id = session["user_id"]
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]  # select only the value inside cash
        updated_cash = current_cash + int(addcash)

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)  # update the cash of the current user

        return redirect("/")
    else:
        return render_template("addcash.html")
