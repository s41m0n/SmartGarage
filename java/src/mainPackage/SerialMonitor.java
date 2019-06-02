package mainPackage;

import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.Charset;

import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent; 
import gnu.io.SerialPortEventListener;

/**
 * Simple Serial Monitor, adaptation from: 
 * 
 * http://playground.arduino.cc/Interfacing/Java
 *
 */
public class SerialMonitor implements SerialPortEventListener {
        private SerialPort serialPort;
        private String s = "";
        private GUI view;
        /**
        * A BufferedReader which will be fed by a InputStreamReader 
        * converting the bytes into characters 
        * making the displayed results codepage independent
        */
        private InputStream input;
        /** The output stream to the port */
        private OutputStream output;
        /** Milliseconds to block while waiting for port open */
        private static final int TIME_OUT = 2000;
        
        public void registerGUI(GUI view) {
            this.view = view;
        }
        
        public void start(String portName, int dataRate) {
                CommPortIdentifier portId = null;
                
                try {
                        portId = CommPortIdentifier.getPortIdentifier(portName);
                        // open serial port, and use class name for the appName.
                        serialPort = (SerialPort) portId.open(this.getClass().getName(),
                                        TIME_OUT);

                        // set port parameters
                        serialPort.setSerialPortParams(dataRate,
                                        SerialPort.DATABITS_8,
                                        SerialPort.STOPBITS_1,
                                        SerialPort.PARITY_NONE);

                        // open the streams
                        // input = new BufferedReader(new InputStreamReader(serialPort.getInputStream()));
                        input = serialPort.getInputStream();
                        output = serialPort.getOutputStream();

                        // add event listeners
                        serialPort.addEventListener(this);
                        serialPort.notifyOnDataAvailable(true);
                } catch (Exception e) {
                        System.err.println(e.toString());
                        System.exit(1);
                }
        }

        /**
         * This should be called when you stop using the port.
         * This will prevent port locking on platforms like Linux.
         */
        public synchronized void close() {
                if (serialPort != null) {
                        serialPort.removeEventListener();
                        serialPort.close();
                }
        }

        /**
         * Handle an event on the serial port. Read the data and print it.
         */
        @Override
        public synchronized void serialEvent(SerialPortEvent oEvent) {
                if (oEvent.getEventType() == SerialPortEvent.DATA_AVAILABLE) {
                        try {
                                Thread.sleep(200);
                                while (input.available() > 0){                          
                                        int inputChar=input.read();
                                        this.s += (char)inputChar;
                                }
                                this.view.notifyNewMessage(this.s);
                                s = "";
                        } catch (Exception e) {
                                System.err.println(e.toString());
                        }
                }
                // Ignore all the other eventTypes, but you should consider the other ones.
        }
        
        /**
         * Method to send messages by serial
         * @param message
         */
        protected synchronized void sendMessage(String message) {
            try {
                this.output.write(message.getBytes(Charset.forName("UTF-8")));
            } catch (Exception ex){
                ex.printStackTrace();
            }
        } 
}