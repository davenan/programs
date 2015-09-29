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

import java.util.ArrayList;

/**
 *  Saves the Answers to each question using an Asynchronous task
 *  Used by AnswerMC , AnswerAD, AnswerSA
 */
public class SaveQuestionAnswers extends AsyncTask<String, Integer, String> {

    @Override
        protected String doInBackground(String... params) {

            // Each param pass from the selected class
            String user = params[0];
            String survey = params[1];
            String qnum = params[2];
            String ques = params[3];
            String answer = params[4];

            // Create name value pairs
            ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
            nameValuePairs.add(new BasicNameValuePair("user", user));
            nameValuePairs.add(new BasicNameValuePair("sid", survey));
            nameValuePairs.add(new BasicNameValuePair("qnum", qnum));
            nameValuePairs.add(new BasicNameValuePair("ques", ques));
            nameValuePairs.add(new BasicNameValuePair("answer", answer));

            // Try an HTTPPOST request, with selected PHP file
            try {
                HttpClient httpclient = new DefaultHttpClient();
                HttpPost httppost = new HttpPost("http://surveytaker.byethost31.com/android_saveq.php");
                httppost.setEntity(new UrlEncodedFormEntity(nameValuePairs));
                HttpResponse response = httpclient.execute(httppost);
                HttpEntity entity = response.getEntity();
            } catch (Exception e) {
                Log.e("log_tag", "Error in http connection " + e.toString());
            }

            // Saving answers so we dont need to return anything
            return null;
        }
}

