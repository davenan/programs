package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import java.io.InputStream;
import java.util.ArrayList;


public class AgreeDisagreeActivity extends ActionBarActivity {

    String user_sid;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.create_ad);

        // This button saves a new Agree/Disagree question to DB
        Button b1 = (Button)findViewById(R.id.adsave_button);
        b1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // User just enters a question
                EditText e = (EditText)findViewById(R.id.ad_question);

                // Save question into string
                String ques = e.getText().toString();

                // Get User and Survey ID from past activity
                user_sid = getIntent().getExtras().getString("user_sid");

                // Split into 2 strings
                String[]parts = user_sid.split(":");

                // This was passed from previous activity which is Q #
                int question_num = getIntent().getExtras().getInt("qnum");

                // Set the user and survey ID
                String u_name = parts[0];
                String sid_name = parts[1];


                // Send everything to PHP for processing
                // Now answers are Agree and Disagree
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("user", u_name));
                nameValuePairs.add(new BasicNameValuePair("sid", sid_name));
                nameValuePairs.add(new BasicNameValuePair("qnum", Integer.toString(question_num)));
                nameValuePairs.add(new BasicNameValuePair("ques", ques));
                nameValuePairs.add(new BasicNameValuePair("ans1", "Agree"));
                nameValuePairs.add(new BasicNameValuePair("ans2", "Disagree"));


                // Open the DB Connection
                DBConnector db = new DBConnector();
                InputStream is = db.create_connection(nameValuePairs, 4);


                // Go back to Create survey for more questions
                Intent i = new Intent(getApplicationContext(), CreateSurveyActivity.class);
                i.putExtra("user_sid", user_sid);
                startActivity(i);
            }
        });

    }
}
