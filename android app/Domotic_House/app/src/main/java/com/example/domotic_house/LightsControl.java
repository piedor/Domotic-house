package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;

public class LightsControl extends AppCompatActivity {
public Button home_lights;
public static ToggleButton kitchenBtn;
public static ToggleButton bathBtn;
public static ToggleButton bedBtn;
public static ToggleButton studioBtn;
TextView title;
TextView title_k;
TextView title_ba;
TextView title_be;
TextView title_s;

//mancano le luci esterne

public static void refreshLights(){
    if ( (Lights.kitchenLight==Lvalues.valueOf("on")) && (!LightsControl.kitchenBtn.isChecked()) )
        LightsControl.kitchenBtn.toggle();
    if ( (Lights.kitchenLight==Lvalues.valueOf("off")) && (LightsControl.kitchenBtn.isChecked()) )
        LightsControl.kitchenBtn.toggle();

    if ( (Lights.bathLight==Lvalues.valueOf("on")) && (!LightsControl.bathBtn.isChecked()) )
        LightsControl.bathBtn.toggle();
    if ( (Lights.bathLight==Lvalues.valueOf("off")) && (LightsControl.bathBtn.isChecked()) )
        LightsControl.bathBtn.toggle();

    if ( (Lights.bedLight==Lvalues.valueOf("on")) && (!LightsControl.bedBtn.isChecked()) )
        LightsControl.bedBtn.toggle();
    if ( (Lights.bedLight==Lvalues.valueOf("off")) && (LightsControl.bedBtn.isChecked()) )
        LightsControl.bedBtn.toggle();

    if ( (Lights.studioLight==Lvalues.valueOf("on")) && (!LightsControl.studioBtn.isChecked()) )
        LightsControl.studioBtn.toggle();
    if ( (Lights.studioLight==Lvalues.valueOf("off")) && (LightsControl.studioBtn.isChecked()) )
        LightsControl.studioBtn.toggle();
}
    void languageReset(){
        title=findViewById(R.id.title_lights);
        title_k=findViewById(R.id.kitchen_light);
        title_be=findViewById(R.id.bedroom_light);
        title_s=findViewById(R.id.studio_light);
        title_ba=findViewById(R.id.bath_light);
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

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lights_control);
        home_lights=findViewById(R.id.home_lights);
        home_lights.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(LightsControl.this, MainActivity.class));
                Communication.synchronizeData();
            }
        });
        kitchenBtn= (ToggleButton) findViewById(R.id.kitchen_button);
        bathBtn= (ToggleButton) findViewById(R.id.bath_button);
        bedBtn= (ToggleButton) findViewById(R.id.bedroom_button);
        studioBtn= (ToggleButton) findViewById(R.id.studio_button);


        kitchenBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(kitchenBtn.isChecked())
                    Lights.kitchenLight=Lvalues.valueOf("on");
                    else Lights.kitchenLight=Lvalues.valueOf("off");
                    Communication.changes='y';
                Communication.synchronizeData();
                }
            });
        bathBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(bathBtn.isChecked())
                    Lights.bathLight=Lvalues.valueOf("on");
                else Lights.bathLight=Lvalues.valueOf("off");
                Communication.changes='y';
                Communication.synchronizeData();
            }
        });
        bedBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(bedBtn.isChecked())
                    Lights.bedLight=Lvalues.valueOf("on");
                else Lights.bedLight=Lvalues.valueOf("off");
                Communication.changes='y';
                Communication.synchronizeData();
            }
        });
        studioBtn.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                if(studioBtn.isChecked())
                    Lights.studioLight=Lvalues.valueOf("on");
                else Lights.studioLight=Lvalues.valueOf("off");
                Communication.changes='y';
                Communication.synchronizeData();
            }
        });

        languageReset();
        refreshLights();
    }
}