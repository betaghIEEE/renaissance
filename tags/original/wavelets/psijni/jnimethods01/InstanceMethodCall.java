class InstanceMethodCall {
    private native void nativeMethod(); 
    private void callback() {
        System.out.println("In Java"); 
    } 
    
    private void smokegun () {
    	System.out.println("This is a smoking gun");
    }
    
    private int smokeint (int giveback) 
    {
    	System.out.print("In Java ");
	System.out.println (giveback);
    	giveback = 100;
    	return giveback;
    }
    
    public static void main(String args[]) {
        InstanceMethodCall c = new InstanceMethodCall(); 
        c.nativeMethod(); 
    } 
    static {
        System.loadLibrary("InstanceMethodCall"); 
    } 
} 
