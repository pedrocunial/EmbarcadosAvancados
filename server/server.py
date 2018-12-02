from flask import render_template, Flask
import cv2


app = Flask(__name__)
camera = cv2.VideoCapture(0)


@app.route('/')
def index():
    gen()
    return render_template('index.html')


def gen():
        _, frame = camera.read()
        cv2.imwrite('static/images/frame.png', frame)


if __name__ == '__main__':
    app.run(debug=True)
