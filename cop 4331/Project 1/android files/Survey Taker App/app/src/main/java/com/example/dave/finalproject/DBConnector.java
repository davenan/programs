package com.example.dave.finalproject;

import android.os.StrictMode;
import android.util.Log;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 *  Class is used for other small HTTPPost request
 *  Like the Login, Register and Connector Classes
 *
 */
public class DBConnector{


    // This is a method returns an Input Stream
    // ArrayList nameValuePairs and a integer key value is passed by the caller
    // int key is to switch the PHP files to connect to DB
    public InputStream create_connection(ArrayList<NameValuePair> nameValuePairs, int key)
    {
        // Thread Policy
        StrictMode.ThreadPolicy policy = new StrictMode.ThreadPolicy.Builder().permitAll().build();
        StrictMode.setThreadPolicy(policy);

        // Create an input stream and set to null
        InputStream is = null;

        // get the url by specified key
        String url = getURL(key);

        // Make the HTTPpost Request, using the url string
        try {
            HttpClient httpclient = new DefaultHttpClient();
            HttpPost httppost = new HttpPost(url);
            httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
            HttpResponse response = httpclient.execute(httppost);
            HttpEntity entity = response.getEntity();
            is = entity.getContent();
            Log.e("log_tag", "connection success ");
        } catch (Exception e) {
            Log.e("log_tag", "Error in http connection " + e.toString());
        }

        // Return the Input stream
        return is;
    }


    // Method uses the given input stream and creates a JSON string
    public String get_response(InputStream is)
    {
        // Empty
        String result = null;

        // Create string from Input Stream
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(is,"iso-8859-1"),8);
            StringBuilder sb = new StringBuilder();
            String line = null;
            while ((line = reader.readLine()) != null) {
                sb.append(line);
            }

            // Close stream
            is.close();

            // Create the String
            result = sb.toString();
        }
        catch(Exception e) {
            Log.e("log_tag", "Error converting string " + e.toString());
        }

        // Return our JSON string
        return result;
    }

    // List of all the PHP files, which a integer key uses to switch the PHP file used
    public String getURL(int key){

        String s = null;
        switch(key){
            case 1:
                s = "http://surveytaker.byethost31.com/android_login.php";
                break;
            case 2:
                s = "http://surveytaker.byethost31.com/android_register.php";
                break;
            case 3:
                s = "http://surveytaker.byethost31.com/android_check.php";
                break;
            case 4:
                s = "http://surveytaker.byethost31.com/android_multiple.php";
                break;
            case 5:
                s = "http://surveytaker.byethost31.com/android_take.php";
                break;
            case 6:
                s = "http://surveytaker.byethost31.com/android_last.php";
                break;
            case 7:
                s = "http://surveytaker.byethost31.com/android_saveq.php";
                break;
            case 8:
                s = "http://surveytaker.byethost31.com/android_show.php";
                break;
            default:
                break;
        }
        return s;
    }

}
