package com.nymi.sample.roamingnea;

public class Main {

	public static void main(String[] args) {
		
		if (args.length == 0) {
			printHelp();
			System.exit(0);
		}
	
		if ((!args[0].equals("setup")) && (!args[0].equals("auth"))){
			System.err.println("Unknown command \"" + args[0] + "\"");
			printHelp();
			System.exit(0);
		}
		
		NymiJavaApi napi = new NymiJavaApi();
	    Integer nPort = 9089;
		Integer retcode = napi.init(".", 3, nPort, "127.0.0.1");
		if (retcode != 0) {
			System.out.println("FATAL ERROR: unable to initialize Nymi API. Error code: " + retcode);
			System.exit(1);
		}

		String serviceUrl = "";
		if (args.length > 2)
			serviceUrl = args[3];
		else
			serviceUrl = "http://127.0.0.1:9090/roamingservice";
		ServiceConnector sc = new ServiceConnector(serviceUrl);
		
		if (args[0].equals("setup")) {
			Setup su = new Setup (napi, sc);
			if (!su.run()) {
				System.out.println("Roaming authentication setup failed");
				System.exit(1);
			}
			System.out.println("Roaming authentication setup complete");
		}
		else if (args[0].equals("auth"))
		{
			Authenticate auth = new Authenticate(napi, sc);
			String userResult = auth.run();
			if (!userResult.equals("")) {
				System.out.println("Roaming authentication successful!");
				System.out.println("User: " + userResult);
			}
			else
			{
				System.out.println("Roaming authentication failed.");
			}
		}
		else
			printHelp();
		System.exit(0);
	}
	
	private static void printHelp () {
		System.out.println("Usage: roamingnea <command> [URL]");
		System.out.println("  The command can be either:");
		System.out.println("    setup : Sets up a provisioned band for roaming authentication");
		System.out.println("    auth : Starts authentication of a set-up band and returns the results");
		System.out.println("");
		System.out.println("  URL is optional and represents the base URL of the roaming authentication web service.");
		System.out.println("  If not included it defaults to \"http://127.0.0.1:9090/roamingauth/\"");
		System.out.println("");
	}
	
}
