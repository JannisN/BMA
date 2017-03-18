package com.a3d.michel.objectloader;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Vector;

public class MainActivity extends AppCompatActivity {
    public StringBuilder sb = new StringBuilder();
    /*public float[] normals = new float[18];
    public float[] uvs = new float[26];
    public float[] positions = new float[24];
    public float[][] faces = new float[12][9];*/
    ArrayList<Float> normal = new ArrayList<>();
    ArrayList<Float> uv = new ArrayList<>();
    ArrayList<Float> position = new ArrayList<>();
    ArrayList<Float> face = new ArrayList<>();
    int normalLineCount = 0;
    int uvLineCount = 0;
    int posLineCount = 0;
    int faceLineCount = 0;
    int normalSize = 0;
    int uvSize = 0;
    int posSize = 0;
    int faceSize = 0;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        Context context = getApplicationContext();
        int filename = R.raw.untitled7;
        int count = 0;
        int i;
        int j;
        String txt;
        String str = read_file(context, filename);
        TextView tvnormals = (TextView)findViewById(R.id.print);

        loadFromString(str);


        float[][] normals = buildArray(normalLineCount, normalSize, normal);
        float[][] uvs = buildArray(uvLineCount, uvSize, uv);
        float[][] positions = buildArray(posLineCount, posSize, position);
        float[][] faces = buildArray(faceLineCount, faceSize, face);


        int face = 0;
        float[] posSorted = new float[faceLineCount*posSize*3];
        float[] uvSorted = new float[faceLineCount*uvSize*3];
        float[] normalSorted = new float[faceLineCount*normalSize*3];

        count = 0;
        for(int k = 0; k < faceLineCount; k++) {
            for (i = 0; i < faceSize / 3; i++) {
                face = (int) faces[k][i];
                for (j = 0; j < posSize; j++) {
                    posSorted[count] = positions[face - 1][j];
                    count++;
                }
            }
        }
        count = 0;
        for(int k = 0; k < faceLineCount; k++) {
            for (i = 3; i < 2*faceSize / 3; i++) {
                face = (int) faces[k][i];
                for (j = 0; j < uvSize; j++) {
                    uvSorted[count] = uvs[face - 1][j];
                    count++;
                }
            }
        }
        count = 0;
        for(int k = 0; k < faceLineCount; k++) {
            for (i = 6; i < 3*faceSize / 3; i++) {
                face = (int) faces[k][i];
                for (j = 0; j < normalSize; j++) {
                    normalSorted[count] = normals[face - 1][j];
                    count++;
                }
            }
        }
        /*face = (int) faces[0][7];
        res[0] = positions[face - 1][2];*/

        txt = Arrays.toString(uvSorted);
        tvnormals.setText(txt);

    }
    public float[][] buildArray(int lineCount, int size, ArrayList<Float> list){
        float[][] array = new float[lineCount][size];
        int count = 0;
        for(int i = 0; i < lineCount; i++){
            for(int j = 0; j < size; j++){
                array[i][j] = list.get(count);
                count++;
            }
        }
        return array;
    }

    public String read_file(Context context, int filename) {
        try {
            InputStream fis = context.getResources().openRawResource(filename);
            InputStreamReader isr = new InputStreamReader(fis, "UTF-8");
            BufferedReader bufferedReader = new BufferedReader(isr);
            String line;
            while ((line = bufferedReader.readLine()) != null) {
                sb.append(line).append("\n");
            }
            return sb.toString();
        } catch (FileNotFoundException e) {
            return "";
        } catch (UnsupportedEncodingException e) {
            return "";
        } catch (IOException e) {
            return "";
        }
    }
    public void loadFromString(String source){
        String lines[] = source.split("\n");
        for(String line : lines ) {
            String normalPrefix = "vn ";
            String uvPrefix = "vt ";
            String vectorPrefix = "v ";
            String facePrefix = "f ";
            if (line.substring(0, normalPrefix.length()).equals(normalPrefix)) {
                String argumentValue = line.substring(normalPrefix.length());
                String[] splits = argumentValue.split(" "); //Einzelne Zahlen in Array gespeichert
                normalLineCount++;
                normalSize = splits.length;
                for (int i = 0; i < splits.length; i++) {
                    normal.add(Float.parseFloat(splits[i]));
                }
            } else if (line.substring(0, uvPrefix.length()).equals(uvPrefix)) {
                String argumentValue = line.substring(uvPrefix.length());
                String[] splits = argumentValue.split(" "); //Einzelne Zahlen in Array gespeichert
                uvLineCount++;
                uvSize = splits.length;
                for (int i = 0; i < splits.length; i++) {
                    uv.add(Float.parseFloat(splits[i]));
                }
            } else if (line.substring(0, vectorPrefix.length()).equals(vectorPrefix)) {
                String argumentValue = line.substring(vectorPrefix.length());
                String[] splits = argumentValue.split(" "); //Einzelne Zahlen in Array gespeichert
                posLineCount++;
                posSize = splits.length;
                for (int i = 0; i < splits.length; i++) {
                    position.add(Float.parseFloat(splits[i]));
                }
            } else if (line.substring(0, facePrefix.length()).equals(facePrefix)) {
                String argumentValue = line.substring(facePrefix.length());
                String[] splits = argumentValue.split(" "); //Einzelne Zahlen in Array gespeichert
                String[] value1 = splits[0].split("/");
                String[] value2 = splits[1].split("/");
                String[] value3 = splits[2].split("/");
                faceLineCount++;
                faceSize = value1.length + value2.length + value3.length;
                for (int i = 0; i < splits.length; i++) {
                    face.add(Float.parseFloat(value1[i]));
                    face.add(Float.parseFloat(value2[i]));
                    face.add(Float.parseFloat(value3[i]));
                }
            }
        }
    }
}
