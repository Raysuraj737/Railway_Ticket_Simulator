from flask import Flask, render_template, request
import subprocess
import os
import random
import csv
from datetime import datetime

app = Flask(__name__)

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
C_EXEC = [os.path.join(BASE_DIR, "railway.exe")]

# ================= UTILITY =================
def generate_pnr():
    """Generate IRCTC-style ticket number"""
    date_part = datetime.now().strftime("%Y%m%d")
    rand_part = random.randint(1000, 9999)
    return f"PNR-{date_part}-{rand_part}"

# ================= ROUTES =================
@app.route("/")
def index():
    return render_template("index.html")

@app.route("/add", methods=["GET", "POST"])
def add_passenger():
    passengers = []
    ticket_no = ""

    if request.method == "POST":
        names = request.form.getlist("name[]")
        ages = request.form.getlist("age[]")
        srcs = request.form.getlist("src[]")
        dests = request.form.getlist("dest[]")
        berths = request.form.getlist("berth[]")

        ticket_no = generate_pnr()

        for i in range(len(names)):
            data = (
                "1\n"
                f"{names[i]}\n"
                f"{ages[i]}\n"
                f"{srcs[i]}\n"
                f"{dests[i]}\n"
                f"{berths[i]}\n"
                "5\n"
            )
            subprocess.run(C_EXEC, input=data, text=True, capture_output=True)

        # Read last added passengers from CSV
        try:
            with open("railway.csv", "r") as file:
                lines = file.readlines()
                for line in lines[-len(names):]:
                    pid, age, name, src, dest, berth, status = line.strip().split(",")
                    passengers.append({
                        "id": pid,
                        "name": name,
                        "age": age,
                        "src": src,
                        "dest": dest,
                        "berth": berth,
                        "status": status
                    })
        except FileNotFoundError:
            pass

        return render_template("add.html", success=True, passengers=passengers, ticket_no=ticket_no)

    return render_template("add.html", success=False, passengers=[], ticket_no="")

# ---------------- SEARCH ----------------
@app.route("/search", methods=["GET", "POST"])
def search():
    passengers = []
    not_found = False

    if request.method == "POST":
        search_id = request.form.get("id", "").strip()
        search_name = request.form.get("name", "").strip()

        if search_id == "":
            search_id = "0"

        data = f"3\n{search_id}\n{search_name}\n5\n"

        result = subprocess.run(C_EXEC, input=data, text=True, capture_output=True)
        output = result.stdout.strip()

        if output == "" or "NOT_FOUND" in output:
            not_found = True
        else:
            for line in output.splitlines():
                parts = line.split(",")
                if len(parts) == 7:
                    passengers.append({
                        "id": parts[0],
                        "age": parts[1],
                        "name": parts[2],
                        "src": parts[3],
                        "dest": parts[4],
                        "berth": parts[5],
                        "status": parts[6]
                    })

    return render_template("search.html", passengers=passengers, not_found=not_found)

# ---------------- CANCEL ----------------
@app.route("/cancel", methods=["GET", "POST"])
def cancel_passenger():
    not_valid = False
    success = False

    if request.method == "POST":
        data = f"2\n{request.form['id']}\n5\n"

        result = subprocess.run(C_EXEC, input=data, text=True, capture_output=True)
        output = result.stdout

        if "Invalid" in output:
            not_valid = True
        elif "CANCEL_SUCCESS" in output:
            success = True

    return render_template("cancel.html", success=success, not_valid=not_valid)

# ---------------- HISTORY ----------------
def display_history():
    history_data = []
    try:
        with open("history.csv", "r") as file:
            reader = csv.reader(file)
            next(reader, None)
            for row in reader:
                history_data.append(row)
    except FileNotFoundError:
        pass
    return history_data

@app.route("/history")
def history():
    return render_template("history.html", history=display_history())

# ---------------- DISPLAY ----------------
@app.route("/display")
def display_passengers():
    lst = []
    try:
        with open("railway.csv", "r") as file:
            for line in file:
                pid, age, name, src, dest, berth, status = line.strip().split(",")
                lst.append({
                    "id": pid,
                    "age": age,
                    "name": name,
                    "src": src,
                    "dest": dest,
                    "berth": berth,
                    "status": status
                })
    except FileNotFoundError:
        pass
    return render_template("display.html", lst=lst)

if __name__ == "__main__":
    app.run(debug=True)
