package com.lonelystudios.rinneganv2;

import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;

import org.opencv.android.JavaCameraView;

import java.io.FileOutputStream;
import java.util.List;
import java.util.ListIterator;

public class OpenCVBestResolutionView extends JavaCameraView implements PictureCallback {

    private static final String TAG = "Sample::Tutorial3View";
    private String mPictureFileName;

    public OpenCVBestResolutionView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public List<String> getEffectList() {
        return mCamera.getParameters().getSupportedColorEffects();
    }

    public boolean isEffectSupported() {
        return (mCamera.getParameters().getColorEffect() != null);
    }

    public String getEffect() {
        return mCamera.getParameters().getColorEffect();
    }

    public void setEffect(String effect) {
        Camera.Parameters params = mCamera.getParameters();
        params.setColorEffect(effect);
        mCamera.setParameters(params);
    }

    public List<Size> getResolutionList() {
        return mCamera.getParameters().getSupportedPreviewSizes();
    }

    public Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.05; //0.05
        final double SECOND_ASPECT_TOLERANCE = 0.5;
        double targetRatio = (double) w/h;

        if (sizes==null) return null;

        Size optimalSize = null;

        double minDiff = Double.MAX_VALUE;

        // Generate an iterator. Start just after the last element.
        ListIterator<Size> li = sizes.listIterator(sizes.size());
        Size size;

        // Iterate in reverse.
        while(li.hasPrevious()) {
            size = li.previous();
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
            if(size.width >= 800) continue;
            if (Math.abs(size.width - w) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.width - w);
            }
        }

        if(optimalSize != null) return optimalSize;

        li = sizes.listIterator(sizes.size());
        while(li.hasPrevious()) {
            size = li.previous();
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > SECOND_ASPECT_TOLERANCE) continue;
            if(size.width >= 800) continue;
            if (Math.abs(size.width - w) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.width - w);
            }
        }


        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size2 : sizes) {
                if (Math.abs(size2.width - w) < minDiff) {
                    optimalSize = size2;
                    minDiff = Math.abs(size2.width - w);
                }
            }
        }

        Log.i(TAG, "Optimizal size, Width: " + optimalSize.width + " , Height: " + optimalSize.height);

        return optimalSize;
    }

    public void setResolution(Size resolution) {
        disconnectCamera();
        mMaxHeight = resolution.height;
        mMaxWidth = resolution.width;
        connectCamera(getWidth(), getHeight());
    }

    public Size getResolution() {
        return mCamera.getParameters().getPreviewSize();
    }

    public void takePicture(final String fileName) {
        Log.i(TAG, "Taking picture");
        this.mPictureFileName = fileName;
        // Postview and jpeg are sent in the same buffers if the queue is not empty when performing a capture.
        // Clear up buffers to avoid mCamera.takePicture to be stuck because of a memory issue
        mCamera.setPreviewCallback(null);

        // PictureCallback is implemented by the current class
        mCamera.takePicture(null, null, this);
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {
        Log.i(TAG, "Saving a bitmap to file");
        // The camera preview was automatically stopped. Start it again.
        mCamera.startPreview();
        mCamera.setPreviewCallback(this);

        // Write the image in a file (in jpeg format)
        try {
            FileOutputStream fos = new FileOutputStream(mPictureFileName);

            fos.write(data);
            fos.close();

        } catch (java.io.IOException e) {
            Log.e("PictureDemo", "Exception in photoCallback", e);
        }

    }
}
