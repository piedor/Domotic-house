package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageView;
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

import java.util.Timer;

public class ThermostatControl extends AppCompatActivity {
public Button home_therm;
public static Button modeT;
public static ToggleButton power_therm;
public static Button plus_therm;
public static Button minus_therm;
private static TextView tempText = null;
private static TextView tempIRL;
private static ImageView fire;
private static ImageView ice;
TextView title;
long timerTemp=2000;    //it's usefull to wait to send the Setted Temperature, so if it's pressed multiples times it only send it once
CountDownTimer countDownTemp=null;

//method to create and start the timer; when the timer ends it call the receiving method and reset it
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
//method to reset the timer
public void timerReset(){
    if(Communication.countDownTimer!=null) {
        Communication.countDownTimer.cancel();
        Communication.receivingTimer=3000;
        startTimer();
    }
    else{startTimer();
    }
}

public void startTempTimer(){
    countDownTemp=new CountDownTimer(timerTemp, 1000) {
        @Override
        public void onTick(long l) {
            timerTemp=l;
        }

        @Override
        public void onFinish() {
            setValuetoSpreadsheet("TSet="+Communication.TMaster.getTempImp());
        }
    }.start();
}

public void resetTempTimer(){
    if(countDownTemp!=null) {
        countDownTemp.cancel();
        timerTemp=2000;
        startTempTimer();
    }
    else{startTempTimer();
    }
}

public void getValuesFromSpreadsheet() {
String url=Communication.URL+"read";
StringRequest stringRequest = new StringRequest(Request.Method.GET, url, new Response.Listener<String>() {
    @Override
    public void onResponse(String response) {
        try {
            Communication.retryTimes=0;
            Communication.insertValues(response, 't');
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
                    home_therm.callOnClick();
                }
            }
        });
int socketTimeOut=5000;
RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
stringRequest.setRetryPolicy(retryPolicy);
RequestQueue queue= Volley.newRequestQueue(this);
queue.add(stringRequest);
matchingThermostat();
}

public void setValuetoSpreadsheet(String value) {
    StringRequest stringRequest=new StringRequest(Request.Method.GET, Communication.createUrl(value), new Response.Listener<String>() {
        @Override
        public void onResponse(String response) {
        }
    },
            new Response.ErrorListener() {
                @Override
                public void onErrorResponse(VolleyError error) {
                }
            });
    int socketTimeOut=5000;
    RetryPolicy retryPolicy=new DefaultRetryPolicy(socketTimeOut, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT);
    stringRequest.setRetryPolicy(retryPolicy);
    RequestQueue queue= Volley.newRequestQueue(this);
    queue.add(stringRequest);
    timerReset();
}

void languageReset(){
        title=findViewById(R.id.thermostat_title);
        if(Communication.language=="ita") {
            title.setText("PANNELLO DI CONTROLLO TERMOSTATO");
        }else{
            title.setText("THERMOSTAT CONTROL PANNEL");
        }
    }

public static void matchingThermostat(){
    if ( (Communication.TMaster.getSwitchTherm()=="on" || Communication.TMaster.getSwitchTherm()=="go_on") && (!ThermostatControl.power_therm.isChecked()) )
        ThermostatControl.power_therm.toggle();
    if ( (Communication.TMaster.getSwitchTherm()=="off" || Communication.TMaster.getSwitchTherm()=="go_off") && (ThermostatControl.power_therm.isChecked()) )
        ThermostatControl.power_therm.toggle();

    if(Communication.TMaster.getSwitchTherm()=="on"|| Communication.TMaster.getSwitchTherm()=="go_on") {
        if (Communication.TMaster.getModeTherm()== "warm") {
            fire.setVisibility(View.VISIBLE);
            ice.setVisibility(View.INVISIBLE);
        } else {
            fire.setVisibility(View.INVISIBLE);
            ice.setVisibility(View.VISIBLE);
        }
        modeT.setVisibility(View.VISIBLE);
        plus_therm.setVisibility(View.VISIBLE);
        minus_therm.setVisibility(View.VISIBLE);
        tempText.setVisibility(View.VISIBLE);
    }else{
        modeT.setVisibility(View.INVISIBLE);
        fire.setVisibility(View.INVISIBLE);
        ice.setVisibility(View.INVISIBLE);
        plus_therm.setVisibility(View.INVISIBLE);
        minus_therm.setVisibility(View.INVISIBLE);
        tempText.setVisibility(View.INVISIBLE);
    }
    tempTextRefresh();
}

public static void tempTextRefresh(){
    tempText.setText("Temp: "+String.format("%.2f", Communication.TMaster.getTempImp()));
    tempIRL.setText("Temp IRL: "+ String.format("%.2f", Communication.TMaster.getTempSens()));
}

public void setTempImp(float i) {
    Communication.TMaster.setTempImp(i);
    tempText.setText("Temp: "+Communication.TMaster.getTempImp());
}

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_thermostat_control);

        modeT=findViewById(R.id.mode_therm);
        tempText=findViewById(R.id.tempText);
        tempIRL=findViewById(R.id.realTemp);
        minus_therm=findViewById(R.id.minus_therm);
        plus_therm=findViewById(R.id.plus_therm);
        fire=findViewById(R.id.fire);
        ice=findViewById(R.id.ice);
        power_therm=findViewById(R.id.on_off);

        getValuesFromSpreadsheet();
        timerReset();

        languageReset();
        matchingThermostat();
        power_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(power_therm.isChecked())
                    Communication.TMaster.setOnTherm("go_on");
                else Communication.TMaster.setOnTherm("go_off");
                matchingThermostat();
                setValuetoSpreadsheet("TSwitch="+Communication.TMaster.getSwitchTherm());
            }
        });

        plus_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setTempImp((float) (Communication.TMaster.getTempImp()+0.5));
                resetTempTimer();
            }
        });

        minus_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                setTempImp((float)(Communication.TMaster.getTempImp()-0.5));
                resetTempTimer();
            }
        });

        modeT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(Communication.TMaster.getModeTherm()=="warm") {
                    Communication.TMaster.setModeTherm("cool");
                }else {
                    Communication.TMaster.setModeTherm("warm");
                }
                matchingThermostat();
                setValuetoSpreadsheet("TMode="+Communication.TMaster.getModeTherm());
            }
        });

        home_therm=findViewById(R.id.home_therm);
        home_therm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Communication.countDownTimer.cancel();
                Communication.receivingTimer=3000;
                startActivity(new Intent(ThermostatControl.this, MainActivity.class));
            }
        });

    }
}