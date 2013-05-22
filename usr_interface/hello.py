from flask import Flask, render_template, request, jsonify
import serial
import json
TTY_PORT = '/dev/ttyUSB0'
BAUD = 9600

s = serial.Serial(TTY_PORT, BAUD, timeout=1)

app = Flask(__name__)
@app.route('/_new')
def new():
        adc = request.args.get('ADC_COMMAND','1', type=int)
        if adc== 1:
            s.write(chr(0x1D))   
            response = s.read(255)
            # inter = jsonify(response)
            new = ([("%d" % ord(char)) for char in response])
            one = new[0]
            volt = {'key':one}
            print (volt)
            
            return jsonify(volt)	

@app.route('/')
def main():
    #  if request.method == "GET":
        
        a = request.args.get('ledred','0', type=int)
        if a == 1:
              s.write(chr(0x01))
        elif a == 0:
             s.write(chr(0x11))
        b = request.args.get('ledwhite','0',type=int)
        if b == 1:
              s.write(chr(0x03))
        elif b == 0:
              s.write(chr(0x1B))
        c = request.args.get('ledyellow','0',type=int)
        if c == 1:
             s.write(chr(0x04))
        elif c == 0:
             s.write(chr(0x1C))
        d = request.args.get('ledgreen','0', type=int)
        if d == 1:
             s.write(chr(0x02))
        elif d == 0:
             s.write(chr(0x1A))
        return render_template('test.html')




if __name__ == '__main__':
    app.run(debug=True)

