import os
import time
import glob
from flask import Flask, redirect, render_template, request, send_file

# Configure Application
app = Flask(__name__)

global filename
global ftype

@app.route("/")
def home():
    # Delete old files
    filelist = glob.glob('uploads/*')
    for f in filelist:
        os.remove(f)
    filelist = glob.glob('downloads/*')
    for f in filelist:
        os.remove(f)

    if request.args.get("home")=="":
        return render_template("home.html")
    
    return render_template("intro.html")

app.config["FILE_UPLOADS"] = "./uploads"

@app.route("/compress", methods=["GET", "POST"])
def compress():

    if request.method == "GET":
        return render_template("compress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system("mv ./uploads/* ./uploads/file.txt")
            os.system("g++ compress.cpp")
            os.system("./a.out > ./downloads/file-compressed.bin")

            return render_template("compress.html", check=1)

        else:
            print("ERROR")
            return render_template("compress.html", check=-1)

@app.route("/decompress", methods=["GET", "POST"])
def decompress():

    if request.method == "GET":
        return render_template("decompress.html", check=0)

    else:
        up_file = request.files["file"]

        if len(up_file.filename) > 0:
            global filename
            global ftype
            filename = up_file.filename
            print(up_file.filename)
            up_file.save(os.path.join(app.config["FILE_UPLOADS"], filename))
            os.system("mv ./uploads/* ./uploads/file-compressed.bin")
            os.system("g++ decompress.cpp")
            os.system("./a.out > ./downloads/file.txt")
            

            return render_template("decompress.html", check=1)

        else:
            print("ERROR")
            return render_template("decompress.html", check=-1)



@app.route("/download-decomp")
def download():
    global filename
    global ftype
    path = "downloads/" + "file.txt"
    return send_file(path, as_attachment=True)


@app.route("/download")
def download_file():
    global filename
    global ftype
    path = "downloads/" + "file-compressed.bin"
    return send_file(path, as_attachment=True)




# Restart application whenever changes are made
if __name__ == "__main__":
    app.run(debug = True, port="8000")