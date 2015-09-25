package com.example.mc.myapplication;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Color;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.util.EncodingUtils;
import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;


public class MainActivity extends FragmentActivity implements TaskFragment.TaskCallbacks{
    private TaskFragment mTaskFragment;
    String[] AccntCredential;
    String locationSetting="";
    String[] registrationinfo;
    private int cursteps=0;
    private ProgressDialog progress;
    private String recipient="";
    Timer myTimer;
    Boolean newmessage=false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        FragmentManager fm = getSupportFragmentManager();
        mTaskFragment = (TaskFragment) fm.findFragmentByTag("task");
        if (mTaskFragment == null) {
            mTaskFragment = new TaskFragment();
            fm.beginTransaction().add(mTaskFragment, "task").commit();
        }
        displayHome();
        LocationManager locationManager = (LocationManager) getSystemService(getApplicationContext().LOCATION_SERVICE);
        LocationListener locationListener = new currentLocation();
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 5000, 10, locationListener);
    }
    private void displayHome(){
        cursteps=0;
        setContentView(R.layout.activity_main);
        Button login=(Button)findViewById(R.id.loginBTN);
        login.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                String param = "username=" + ((EditText) findViewById(R.id.username)).getText().toString();
                param += "&password=" + ((EditText) findViewById(R.id.password)).getText().toString();
                mTaskFragment.start(param);
                return false;
            }
        });
        Button register=(Button)findViewById(R.id.register);
        register.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                cursteps = 1;
                loadRegiterStep();
                return false;
            }
        });

    }
    private void processRegistration(String response){
        if(cursteps==1){
            try{
                JSONObject jsonObj = new JSONObject(response);
                if(jsonObj.getInt("status")==1){
                    registrationinfo=new String[3];
                    registrationinfo[0]=((EditText)findViewById(R.id.firstname)).getText().toString();
                    registrationinfo[1]=((EditText)findViewById(R.id.lastname)).getText().toString();
                    registrationinfo[2]=((EditText)findViewById(R.id.phone)).getText().toString();
                    cursteps=2;
                    loadRegiterStep();

                }else{
                    Toast.makeText(getApplicationContext(),jsonObj.getString("error"),Toast.LENGTH_LONG).show();
                }
            }catch (Exception e){
                Toast.makeText(getApplicationContext(),"An unknown error has occured please try again later" ,Toast.LENGTH_LONG).show();
            }
        }
        else{
            try{
                JSONObject jsonObj = new JSONObject(response);
                if(jsonObj.getInt("status")==1){
                    AccntCredential=new String[2];
                    AccntCredential[0]=jsonObj.getString("id");
                    AccntCredential[1]=jsonObj.getString("token");
                    displayWebview();
                }else{
                    Toast.makeText(getApplicationContext(),  jsonObj.getString("error"),Toast.LENGTH_LONG).show();
                }
            }catch (Exception e){
                Toast.makeText(getApplicationContext(), "An unknown error has occured please try again later" ,Toast.LENGTH_LONG).show();
            }
        }
    }
    class currentLocation implements LocationListener {

        @Override
        public void onLocationChanged(Location loc) {
            locationSetting="setLon="+ loc.getLongitude()+"&setLat="+ loc.getLatitude();
        }

        @Override
        public void onProviderDisabled(String provider) {}

        @Override
        public void onProviderEnabled(String provider) {}

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {}
    }
    private void loadRegiterStep(){
        if(cursteps==1){
            setContentView(R.layout.register);
            Button back=(Button)findViewById(R.id.backToHome);
            back.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    displayHome();
                    return false;
                }
            });
            Button next=(Button)findViewById(R.id.submitStepone);
            next.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    String param = "check_phone=" + ((EditText) findViewById(R.id.phone)).getText().toString();
                    mTaskFragment.start(param);
                    return false;
                }
            });
        }else{
            setContentView(R.layout.password);
            Button back=(Button)findViewById(R.id.backPassword);
            back.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    cursteps=1;
                    loadRegiterStep();
                    return false;
                }
            });
            Button next=(Button)findViewById(R.id.register);
            next.setOnTouchListener(new View.OnTouchListener() {
                @Override
                public boolean onTouch(View v, MotionEvent event) {
                    String param = "register=y&username=" + ((EditText) findViewById(R.id.username)).getText().toString();
                    param += "&password=" + ((EditText) findViewById(R.id.password)).getText().toString();
                    param += "&confirmpassword=" + ((EditText) findViewById(R.id.confirmpassword)).getText().toString();
                    param += "&firstname=" + registrationinfo[0];
                    param += "&lastname=" + registrationinfo[1];
                    param += "&phone=" + registrationinfo[2];
                    mTaskFragment.start(param);
                    return false;
                }
            });
        }
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onPreExecute() {
        if(cursteps!=5)
            progress = ProgressDialog.show(this, "Progress","Login in", true);
    }

    @Override
    public void onProgressUpdate(int percent) {

    }
    public void displayWebview(){
        setContentView(R.layout.webview);
        myTimer = new Timer();
        myTimer.scheduleAtFixedRate(new MyTimerTask(), 0, 5000);
        WebView webview = new WebView(this);
        webview.setWebViewClient(new WebViewClient());
        WebSettings webSettings = webview.getSettings();
        webSettings.setJavaScriptEnabled(true);
        setContentView(webview);
        byte[] post = EncodingUtils.getBytes("id="+AccntCredential[0]+"&token="+AccntCredential[1], "BASE64");
        webview.postUrl("http://project.bbr.ms/app/index.php", post);
    }
    @Override
    public void onCancelled() {

    }
    class MyTimerTask extends TimerTask {
        @Override
        public void run() {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    String param=locationSetting+"&id="+AccntCredential[0]+"&token="+AccntCredential[1];
                    cursteps=5;
                    mTaskFragment.start(param);

                }
            });
        }
    }
    private void processLogin(String response){
        try{
            JSONObject jsonObj = new JSONObject(response);
            if(jsonObj.getInt("status")==1){
                AccntCredential=new String[2];
                AccntCredential[0]=jsonObj.getString("id");
                AccntCredential[1]=jsonObj.getString("token");
                displayWebview();
            }else{
                Toast.makeText(getApplicationContext(), "Invalid login credenials" ,Toast.LENGTH_LONG).show();
            }
        }catch (Exception e){
            Toast.makeText(getApplicationContext(), "An unknown error has occured please try again later" ,Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onPostExecute() {
        if(cursteps!=5)
            progress.dismiss();
        String response=mTaskFragment.Response;
        if(!response.equals("fail")){
            if (cursteps==0)
                processLogin(response);
            else if(cursteps<3)
                processRegistration(response);
        } else{
            Toast.makeText(getApplicationContext(), "An unknown error has occured please try again later" ,Toast.LENGTH_LONG).show();
        }
    }
}
