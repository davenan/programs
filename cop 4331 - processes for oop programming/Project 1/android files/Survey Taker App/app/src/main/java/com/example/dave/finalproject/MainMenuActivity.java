package com.example.dave.finalproject;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class MainMenuActivity extends ActionBarActivity {

    String droid_user;

    // The main menu after the user gets a successful login...
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_menu);

        // So we get the user
        droid_user = getIntent().getExtras().getString("user");

        // The user wants to create a new survey to add to DB
        Button create_survey = (Button)findViewById(R.id.create_survey_button);
        create_survey.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {

                // Display user name to screen (more for debug purposes)
                Toast.makeText(getApplicationContext(), "User : "+ droid_user, Toast.LENGTH_LONG).show();

                // Open the Create Survey Connector and pass over the user along with the Intent
                Intent i = new Intent(getApplicationContext(), CreateSurveyConnector.class);
                i.putExtra("user", droid_user);
                startActivity(i);
                finish();
            }
        });

        // The user wants to take a new survey to add to DB
        // Same setup here as the create process found above
        Button take_survey = (Button)findViewById(R.id.take_survey_button);
        take_survey.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(getApplicationContext(), "User : "+ droid_user, Toast.LENGTH_LONG).show();
                Intent i = new Intent(getApplicationContext(), TakeSurveyConnector.class);
                i.putExtra("user", droid_user);
                startActivity(i);
                finish();
            }
        });


        /*-------------------------Did not get around to it -------------------------------------------*/
        Button show_results = (Button)findViewById(R.id.show_survey_results_button);
        show_results.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(getApplicationContext(), ShowSurveyConnector.class);
                i.putExtra("user", droid_user);
                startActivity(i);
            }
        });


        // The user wants to logout from their account, so redirect back to Login Page
        Button logout = (Button)findViewById(R.id.logout_button);
        logout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent i = new Intent(getApplicationContext(), LoginActivity.class);
                startActivity(i);
                Toast.makeText(getApplicationContext(), "Logout Successful", Toast.LENGTH_SHORT).show();
            }
        });
    }
}
