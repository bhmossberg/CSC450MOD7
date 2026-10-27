public class LoginService {
	public boolean authenticate(String username, char[] password) {
		// password exists only inside this method
		boolean match = checkHash(username, password);
		
		// wipe the array before the method returns
		java.util.Arrays.fill(password, '\0');
		
		return match;
		// password is out of scope here
	}

	private boolean checkHash(String username, char[] password) {
		// password is still only the parameter of this method
		// a real implementation would hash and compare, then return
		return false;
	}
}