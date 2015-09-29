package com.example.dave.finalproject;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;

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
import java.util.StringTokenizer;


public class TakeSurveyConnector extends ActionBarActivity {

    /* This activity is used ONLY to query the database and populate a list view
       with all of the survey names, created by all the users in the DB and place them to
       the UI. Once the user selects the survey to take, the class AnswerMCActivity
       is called and the user can then take the questions for the specified survey.
     */

    // List view for populating all surveys
    ListView listval;

    // Adapter to populate list view
    ArrayAdapter<String> adapter;

    // Holds end values.
    ArrayList<String> list = new ArrayList<>();

    // Our user
    String user;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.take_connector);

        // Create the list view
        listval = (ListView)findViewById(R.id.take_survey_list);

        // Create the adapter to populate list view
        adapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1, android.R.id.text1, list);
/*
        // Nothing to send, instead we just query all rows directly
        ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();

        // Connect to the database
        DBConnector db = new DBConnector();

        // Connect to DB and query -> uses TAKE.PHP(5)
        InputStream is = db.create_connection(nameValuePairs, 5);
*/
        // The String result contains only SURVEY NAMES with $ delimiter from DB
        // PHP file used places all of the delimiters for the String result
        // Ex: "$Food" "$Cars"....
        String result = new TakeTask().doInBackground();

        // Remove all quotations -> $Food $Cars
        result = result.replace("\"", "");

        // Tokenize string by the ($) delimiter
        StringTokenizer st2 = new StringTokenizer(result, "$");

        // Add each seperate token to the list
        while (st2.hasMoreElements()) {
            list.add(st2.nextToken());
        }

        // Set survey names to the adapter
        listval.setAdapter(adapter);

        // Get the user from the past activity which was Main Menu
        user = getIntent().getExtras().getString("user");

        // Set listener to check the user survey selection that they want to take
        listval.setOnItemClickListener(new ListView.OnItemClickListener() {

                    @Override
                public void onItemClick(AdapterView<?> parent, View view,
                                    int position, long id){


                        // Setup the next activity
                        Intent i = new Intent(getApplicationContext(), AnswerMCActivity.class);

                        // List contains strings of surveys, the position is the SURVEY
                        // The user chose to take so pass SURVEY name to next activity.
                        i.putExtra("survey", list.get(position));

                        // Load all the questions for the survey into result
                        String result = new LoadQuestions().doInBackground(list.get(position));

                        // Strip all the quotation marks from that string
                        result = result.replace("\"", "");

                        // Pass all the questions from DB to AnswerMCActivity
                        i.putExtra("result", result);

                        // Put the user who taking the survey to next activity
                        i.putExtra("user", user);

                        // Start next activity
                        startActivity(i);
                   }
             });
       }



    public class TakeTask extends AsyncTask<String, String, String>{

        @Override
        protected String doInBackground(String... params) {
            InputStream is = null;

            String url = "http://surveytaker.byethost31.com/android_take.php";

            ArrayList<NameValuePair>nameValuePairs = new ArrayList<>();

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
            String result = null;

            try{
                BufferedReader reader = new BufferedReader(new InputStreamReader(is,"iso-8859-1"),8);
                StringBuilder sb = new StringBuilder();
                String line = null;
                while ((line = reader.readLine()) != null)
                {
                    sb.append(line);
                }
                is.close();
                result=sb.toString();
            }
            catch(Exception e)
            {
                Log.e("log_tag", "Error converting result " + e.toString());
            }
            return result;
        }
    }
}
