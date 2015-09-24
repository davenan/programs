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


public class RegisterActivity extends ActionBarActivity {

    public void onCreate(Bundle savedInstanceState)

    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.register);

            /* User finish button to create a new user */
            Button button = (Button) findViewById(R.id.finish_button);
            button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                // User enters a new user name and password for setup
                EditText editText = (EditText)findViewById(R.id.username);
                String droid_user = editText.getText().toString();
                EditText editText1 = (EditText)findViewById(R.id.password);
                String droid_pass = editText1.getText().toString();

                // Setup credentials for PHP file
                ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
                nameValuePairs.add(new BasicNameValuePair("username",droid_user));
                nameValuePairs.add(new BasicNameValuePair("password",droid_pass));

                // Connect to Database
                DBConnector db = new DBConnector();

                // Once open send the made up username and password uses -> REGISTER.PHP(2)
                InputStream is = db.create_connection(nameValuePairs, 2);

                // Our query result from the PHP file
                String result = db.get_response(is);

                // DEBUG
                Log.e("log_tag", "String returned " + result);

                // Test if there was a blank field, if so user has to enter a user/pass
                if(result.contains("Enter")){
                    Toast.makeText(getApplicationContext(), "Enter a Username/Password", Toast.LENGTH_SHORT).show();
                }

                // If the username created is not in system, then create the user
                else if(!result.contains(droid_user)){

                        // Display success and we return to the Login Activity so the user
                        // can login with newly created name
                        Toast.makeText(getApplicationContext(), "New User Created", Toast.LENGTH_SHORT).show();
                        Intent i = new Intent(getApplicationContext(), LoginActivity.class);
                        startActivity(i);
                }

                // The user is in the system, try another user name
                else{
                    Toast.makeText(getApplicationContext(), "Username already Taken", Toast.LENGTH_SHORT).show();
                }
            }
        });

    }
}