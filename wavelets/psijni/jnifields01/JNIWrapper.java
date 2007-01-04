//
//  JNIWrapper.java
//
//  Created by Daniel Beatty on Mon Jan 05 2004.
//  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
//

import java.util.*;

public class JNIWrapper {

    static {
        // Ensure native JNI library is loaded
        System.loadLibrary("jnifields01");
    }
    private String s;
    
    private native void accessField ();

    public JNIWrapper() {
        System.out.println("JNIWrapper instance created");
    }

    native int native_method(String arg);
    
    

    public static void main (String args[]) {
        // insert code here...
        System.out.println("Started JNIWrapper");
        JNIWrapper newjni = new JNIWrapper();
        int result = newjni.native_method("Hello World !");
        System.out.println("Finished JNIWrapper. Answer is " + result);
        newjni.s ="abc";
        newjni.accessField();
        System.out.println ("In Java");
        System.out.println (" c.s = \"" + newjni.s + "\"");
    }

}
