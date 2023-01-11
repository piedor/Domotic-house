package com.example.domotic_house;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class GateControl extends AppCompatActivity {
public Button home_gate;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gate_control);

        home_gate=findViewById(R.id.home_gate);
        home_gate.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(GateControl.this, MainActivity.class));
            }
        });

    }
}