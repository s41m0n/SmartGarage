package mainPackage;

import javax.swing.SwingUtilities;

/**
 * Main class, GUI and SerialMonito have been launched then this thread does
 * nothing more
 */
public class Application {

    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.out.println("Invalid number of arguments!\t(<CommPortName> <BoundRate>");
            System.exit(1);
        }
        String comPortName = args[0];
        int dataRate = Integer.parseInt(args[1]);
        SerialMonitor monitor = new SerialMonitor();
        GUI view = new GUI();
        monitor.registerGUI(view);
        view.registerMonitor(monitor);
        monitor.start(comPortName, dataRate);
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                view.init();
            }
        });
    }
}