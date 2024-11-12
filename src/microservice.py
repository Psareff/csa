from flask import Flask, jsonify

app = Flask(__name__)

@app.route('/greet/<username>', methods=['GET'])
def greet(username):
    return jsonify(message=f"Hello, {username}!")

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5001)
