package com.example.user.a2d;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.RectF;
import android.view.View;
import android.graphics.Paint;
import android.util.Log;
import android.graphics.RectF;

import java.nio.ByteBuffer;

/**
 * Created by user1 on 09.12.2016.
 */

public class Drawer extends View {
    byte[] data;
    public Drawer(Context context, byte[] d) {
        super(context);
        paint.setColor(Color.WHITE);
        data = d;
    }
    Paint paint = new Paint();

    int ByteToInt(byte[] data, int f){
        int i = ((data[f+3] & 0xFF) << 24) | ((data[f+2] & 0xFF) << 16) | ((data[f+1] & 0xFF) << 8)  |
                (data[f] & 0xFF);
        /*
        Log.d("ByteToInt", ">>");
        Log.d("data[f]", Byte.toString(data[f]));
        Log.d("data[f+1]", Byte.toString(data[f+1]));
        Log.d("data[f+2]", Byte.toString(data[f+2]));
        Log.d("data[f+3]", Byte.toString(data[f+3]));
        Log.d("ByteToInt", "<<");
        */
        return i;
    }

    double ByteToDouble(byte[] data, int f){
        long l = ((data[f+7] & 0xFFL) << 56) | ((data[f+6] & 0xFFL) << 48) | ((data[f+5] & 0xFFL) << 40) |
                ((data[f+4] & 0xFFL) << 32) | ((data[f+3] & 0xFFL) << 24) | ((data[f+2] & 0xFFL) << 16) |
                ((data[f+1] & 0xFFL) << 8)  | (data[f] & 0xFFL);
        /*
        Log.d("ByteToDouble", ">>");
        Log.d("data[f]", Byte.toString(data[f]));
        Log.d("data[f+1]", Byte.toString(data[f+1]));
        Log.d("data[f+2]", Byte.toString(data[f+2]));
        Log.d("data[f+3]", Byte.toString(data[f+3]));
        Log.d("data[f+4]", Byte.toString(data[f+4]));
        Log.d("data[f+5]", Byte.toString(data[f+5]));
        Log.d("data[f+6]", Byte.toString(data[f+6]));
        Log.d("data[f+7]", Byte.toString(data[f+7]));
        Log.d("ByteToDouble", "<<");
        */
        return Double.longBitsToDouble(l);
    }

    @Override
    protected void onDraw(Canvas canvas){
        super.onDraw(canvas);
        int pointCoordSize = data[0];
//        Log.d("intSize", Integer.toString(pointCoordSize));
        int radiusSize = data[1];
//        Log.d("radiusSize", Integer.toString(radiusSize));
        int intSize = data[2];
//        Log.d("intSize", Integer.toString(intSize));
        int circlesCount = ByteToInt(data, 3);
//        Log.d("circlesCount", Integer.toString(circlesCount));
        int circleSize = ByteToInt(data, 3+intSize);
//        Log.d("circleSize", Integer.toString(circleSize));
        for (int i=3+intSize*2; i<circlesCount*circleSize+3+intSize*2; i+=circleSize){
            float x = (float)ByteToDouble(data, i);
//            Log.d("X", Float.toString(x));
            float y = (float)ByteToDouble(data, i+pointCoordSize);
//            Log.d("Y", Float.toString(y));
//            double R = ByteToDouble(data, i+pointCoordSize*2);
            float r = (float)ByteToDouble(data, i+pointCoordSize*2);
//            Log.d("R", Double.toString(R));
            canvas.drawArc(new RectF(x-r,y-r,x+r,y+r),0,360,true,paint);
        }
    }
}


