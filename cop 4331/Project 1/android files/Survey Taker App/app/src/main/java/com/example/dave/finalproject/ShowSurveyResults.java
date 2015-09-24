package com.example.dave.finalproject;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.text.method.ScrollingMovementMethod;
import android.widget.TextView;

import org.apache.http.NameValuePair;
import org.apache.http.message.BasicNameValuePair;

import java.io.InputStream;
import java.util.ArrayList;

public class ShowSurveyResults extends ActionBarActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.show_screen);

        TextView tv = (TextView)findViewById(R.id.message_scroll);
        tv.setMovementMethod(new ScrollingMovementMethod());

        String user = getUser();
        String survey = getSurvey();

        // Send to PHP file for processing
        ArrayList<NameValuePair> nameValuePairs = new ArrayList<NameValuePair>();
        nameValuePairs.add(new BasicNameValuePair("sid", survey));
        DBConnector db = new DBConnector();
        InputStream is = db.create_connection(nameValuePairs, 8);
        String result = db.get_response(is);

        result = result.replace("\"", " ");
        result = result.replace("$", "\n\n");
        result = result.replace("#", "\n");
        result = result.replace(",", "\n");

        tv.setText(result);
    }

    public String getUser() {
        String str = null;
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            str = extras.getString("user");
        }
        return str;
    }


    public String getSurvey() {
        String str = null;
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            str = extras.getString("survey");
        }
        return str;
    }
}
