package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import java.io.InputStream;
import java.util.ArrayList;


/* This activity is only used to create a survey name and
   checks to see if it is a new survey name or one that already
   is in the database. On success it goes to next activity to
   let users create the questions, else just ask user to enter
   a new survey name that isn't in the system. */

public class CreateSurveyConnector extends ActionBarActivity {

    String droid_user;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.create_connector);


        // Button for actions after the user types in the survey name
        Button connect_to_survey = (Button)findViewById(R.id.surveyName_button);
        connect_to_survey.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {

                // Get the user that is creating it from past activity
                droid_user = getIntent().getExtras().getString("user");

                // The survey name the user is trying to create
                EditText e1 = (EditText)findViewById(R.id.write_survey_name);
                String survey_name = e1.getText().toString();

                // Send to PHP file to check if any survey with that name is in the DB
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("user", droid_user));
                nameValuePairs.add(new BasicNameValuePair("sid", survey_name));

                // Open the database connection
                DBConnector db = new DBConnector();

                // Use the credentials to check the name uses -> CHECK.PHP(3)
                InputStream is = db.create_connection(nameValuePairs, 3);

                // Our query results
                String result = db.get_response(is);

                // Debug
                Log.e("log_tag", result);


                // User never typed into survey name field
                if(survey_name.isEmpty()){
                    Toast.makeText(getApplicationContext(), "Enter a Survey Name", Toast.LENGTH_LONG).show();
                }

                // If the DB already has that unique survey name, (String result = false)
                else if(result.contains("false")){
                    Toast.makeText(getApplicationContext(), "Survey Name Exists", Toast.LENGTH_LONG).show();
                }


                // Okay so the survey name is acceptable so we do the following...
                else {

                    // Concatenate the USER and the SURVEY into one String
                    // Format -> (USER:SURVEY) with colon delimiter
                    String user_sid = droid_user + ":" + survey_name;

                    // Open the next intent to start making the questions for their new survey
                    Intent i = new Intent(getApplicationContext(), CreateSurveyActivity.class);

                    // Place the (USER:SURVEY) String to the next activity
                    i.putExtra("user_sid", user_sid);

                    // Finish the connector activity
                    finish();

                    // Start next one Create Survey Activity
                    startActivity(i);
                }
            }
        });
    }
}
