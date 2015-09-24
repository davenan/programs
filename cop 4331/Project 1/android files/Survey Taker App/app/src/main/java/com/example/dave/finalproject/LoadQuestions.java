package com.example.dave.finalproject;

import android.os.AsyncTask;
import android.util.Log;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;

/**
 *  Loads the Questions from DB -> into the TakeSurveyConnector Class
 *
 */
public class LoadQuestions extends AsyncTask<String, Integer, String>{


    @Override
    protected String doInBackground(String... params) {

        // The first param is our Survey Name
        String survey = params[0];

        // Add it to a name value pair
        ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
        nameValuePairs.add(new BasicNameValuePair("sid", survey));

        // Create an input stream
        InputStream is = null;

        // try a httppost request, using specified PHP file
        try {
            HttpClient httpclient = new DefaultHttpClient();
            HttpPost httppost = new HttpPost("http://surveytaker.byethost31.com/android_last.php");
            httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
            HttpResponse response = httpclient.execute(httppost);
            HttpEntity entity = response.getEntity();
            is = entity.getContent();
        } catch (Exception e) {
            Log.e("log_tag", "Error in http connection " + e.toString());
        }

        // Create a result String
        String result = null;

        // Build a string from the result of the InputStream
        try{
            BufferedReader reader = new BufferedReader(new InputStreamReader(is,"iso-8859-1"),8);
            StringBuilder sb = new StringBuilder();
            String line = null;
            while ((line = reader.readLine()) != null){
                sb.append(line);
            }

            // Close the input stream
            is.close();

            // Create the string
            result = sb.toString();
        }
        catch(Exception e)
        {
            Log.e("log_tag", "Error converting string " + e.toString());
        }

        // Return the JSON string
        return result;
    }
}
