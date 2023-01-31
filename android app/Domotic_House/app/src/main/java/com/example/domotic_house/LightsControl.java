package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.RetryPolicy;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;

public class LightsControl extends AppCompatActivity {
private Button home_lights;
private static ToggleButton kitchenBtn;
private static ToggleButton bathBtn;
private static ToggleButton bedBtn;
private static ToggleButton studioBtn;
private static ToggleButton outdoorBtn;
public static TextView title;
static TextView title_k;
TextView title_ba;
TextView title_be;
TextView title_s;
TextView title_o;

    public static void matchingLights(){
    if ( (Communication.LMaster.getKitchenLight()=="on" || Communication.LMaster.getKitchenLight()=="go_on") && (!kitchenBtn.isChecked()) )
        kitchenBtn.toggle();
    if ( (Communication.LMaster.getKitchenLight()=="off" || Communication.LMaster.getKitchenLight()=="go_off") && (kitchenBtn.isChecked()) )
        kitchenBtn.toggle();
    if ( (Communication.LMaster.getBathLight()=="on" || Communication.LMaster.getBathLight()=="go_on") && (!bathBtn.isChecked()) )
        bathBtn.toggle();
    if ( (Communication.LMaster.getBathLight()=="off" || Communication.LMaster.getBathLight()=="go_off") && (bathBtn.isChecked()) )
        bathBtn.toggle();

    if ( (Communication.LMaster.getBedLight()=="on" || Communication.LMaster.getBedLight()=="go_on") && (!bedBtn.isChecked()) )
        bedBtn.toggle();
    if ( (Communication.LMaster.getBedLight()=="off" || Communication.LMaster.getBedLight()=="go_off") && (bedBtn.isChecked()) )
        bedBtn.toggle();

    if ( (Communication.LMaster.getStudioLight()=="on" || Communication.LMaster.getStudioLight()=="go_on") && (!studioBtn.isChecked()) )
        studioBtn.toggle();
    if ( (Communication.LMaster.getStudioLight()=="off" || Communication.LMaster.getStudioLight()=="go_off") && (studioBtn.isChecked()) )
        studioBtn.toggle();
    if ( (Communication.LMaster.getOutdoorLight()=="on") && (!outdoorBtn.isChecked()) )
        outdoorBtn.toggle();
    if (( Communication.LMaster.getOutdoorLight()=="sens_on") && (!outdoorBtn.isChecked()) )
        outdoorBtn.toggle();
    if ( (Communication.LMaster.getOutdoorLight()=="off") && (outdoorBtn.isChecked()) )
        outdoorBtn.toggle();
    if (( Communication.LMaster.getOutdoorLight()=="sens_off") && (outdoorBtn.isChecked()) )
        outdoorBtn.toggle();
}

public void startTimer(){
    Communication.countDownTimer=new CountDownTimer(Communication.receivingTimer, 1000) {
        @Override
        public void onTick(long l) {
            Communication.receivingTimer=l;
        }

        @Override
        public void onFinish() {
            getValuesFromSpreadsheet();
            timerReset();
        }
    }.start();
}

public void timerReset(){
    if(Communication.countDownTimer!=null) {
        Communication.countDownTimer.cancel();
        Communication.receivingTimer=3000;
        startTimer();
    }
    else{startTimer();
    }
}
//method to get values from spreadsheet
public void getValuesFromSpreadsheet() {
    String url=Communication.URL+"read";
    StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
        @Override
        public void onResponse(String response) {
            try {
                Communication.retryTimes=0;
                Communication.insertValues(response, 'l');
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
    },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Toast.makeText(getApplicationContext(),"Connection ERROR", Toast.LENGTH_SHORT ).show();
                    Communication.retryTimes++;
                    if(Communication.retryTimes>5){
                        Communication.retryTimes=0;
                        Toast.makeText(getApplicationContext(),"Too many connection ERRORs,\nCheck your connection, then try again", Toast.LENGTH_SHORT ).show();
                        home_lights.callOnClick();
                    }
                }
            });
    int socketTimeOut=5000;
    RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
    stringRequest.setRetryPolicy(retryPolicy);
    RequestQueue queue= Volley.newRequestQueue(this);
    queue.add(stringRequest);

}

//method to set values to spreadsheet
public void setValuetoSpreadsheet(String value) {
    StringRequest stringRequest=new StringRequest(Request.Method.GET, Communication.setValues(), new Response.Listener<String>() {
        @Override
        public void onResponse(String response) {
        }
    },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                    Toast.makeText(getApplicationContext(),"Connection ERROR", Toast.LENGTH_SHORT ).show();
                }
            });
    int socketTimeOut=5000;
    RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
    stringRequest.setRetryPolicy(retryPolicy);
    RequestQueue queue= Volley.newRequestQueue(this);
    queue.add(stringRequest);
    timerReset();
}

//method to match the language setting
void languageReset(){
    title=findViewById(R.id.title_lights);
    title_k=findViewById(R.id.kitchen_light);
    title_be=findViewById(R.id.bedroom_light);
    title_s=findViewById(R.id.studio_light);
    title_ba=findViewById(R.id.bath_light);
    title_o=findViewById(R.id.outdoor_light);
    if(Communication.language=="ita") {
        title.setText("PANNELLO DI CONTROLLO LUCI");
        title_k.setText("Cucina");
        title_ba.setText("Bagno");
        title_s.setText("Studio");
        title_be.setText("Camera");
    }else{
        title.setText("LIGHTS CONTROL PANNEL");
        title_k.setText("Kitchen");
        title_ba.setText("Bathroom");
        title_s.setText("Studio");
        title_be.setText("Bedroom");
    }
}

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lights_control);

        home_lights=findViewById(R.id.home_lights);
        home_lights.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(LightsControl.this, MainActivity.class));
                Communication.countDownTimer.cancel();
                Communication.receivingTimer=3000;
            }
        });

        kitchenBtn= (ToggleButton) findViewById(R.id.kitchen_button);
        bathBtn= (ToggleButton) findViewById(R.id.bath_button);
        bedBtn= (ToggleButton) findViewById(R.id.bedroom_button);
        studioBtn= (ToggleButton) findViewById(R.id.studio_button);
        outdoorBtn=(ToggleButton) findViewById(R.id.outdoor_button);

        getValuesFromSpreadsheet(); //get the values from the database
        timerReset();
        languageReset();
        matchingLights();

        kitchenBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(kitchenBtn.isChecked())
                    Communication.LMaster.setKitchenLight("go_on");
                else Communication.LMaster.setKitchenLight("go_off");
                setValuetoSpreadsheet("LedKitchen="+Communication.LMaster.getKitchenLight());
            }
        });

        bathBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(bathBtn.isChecked())
                    Communication.LMaster.setBathLight("go_on");
                else Communication.LMaster.setBathLight("go_off");
                setValuetoSpreadsheet("LedBathroom="+Communication.LMaster.getBathLight());
            }
        });

        bedBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(bedBtn.isChecked()){
                    System.out.println("bedroom!");
                    Communication.LMaster.setBedLight("go_on");
                }
                else Communication.LMaster.setBedLight("go_off");
                setValuetoSpreadsheet("LedBedroom="+Communication.LMaster.getBedLight());
            }
        });

        studioBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(studioBtn.isChecked()) {
                    System.out.println("studio!");
                    Communication.LMaster.setStudioLight("go_on");
                }
                else Communication.LMaster.setStudioLight("go_off");
                setValuetoSpreadsheet("LedStudio="+Communication.LMaster.getStudioLight());
            }
        });

        outdoorBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(outdoorBtn.isChecked())
                    Communication.LMaster.setOutdoorLight(Loutdoor.on);
                else Communication.LMaster.setOutdoorLight(Loutdoor.off);
                setValuetoSpreadsheet("LedOutdoor="+Communication.LMaster.getOutdoorLight());
            }
        });

    }
}