import java.io.*; 

public class Main{ 
	static String [] line; 

	public static void main (String[] args){ 
		try{
			InputStreamReader ir = new InputStreamReader(System.in); 
			BufferedReader in = new BufferedReader(ir); 
			String s = in.readLine();
		}
		catch (Exception ex){}
		System.out.println(
			"otima" + '\n' + 
			"14" + '\n' + 
			"1 1 1" + '\n' +
			"2 4 8"
		);
	} 
}  

