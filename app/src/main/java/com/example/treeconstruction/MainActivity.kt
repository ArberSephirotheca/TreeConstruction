package com.example.treeconstruction

import android.content.res.AssetManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.treeconstruction.databinding.ActivityMainBinding
import java.io.BufferedReader
import java.io.IOException
import java.io.InputStreamReader


class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding
    private var sAssetManager: AssetManager? = null
//    override fun onCreate(savedInstanceState: Bundle?) {
//        super.onCreate(savedInstanceState)
//
//        binding = ActivityMainBinding.inflate(layoutInflater)
//        setContentView(binding.root)
//
//        // Example of a call to a native method
//        binding.sampleText.text = stringFromJNI()
//    }
override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    if (sAssetManager == null) {
        sAssetManager = getAssets();
    }
    binding = ActivityMainBinding.inflate(layoutInflater)
    setContentView(binding.root)

    // redirect the stdout
    openFileAndRedirectStdout()
    // Call the android_main function
    sAssetManager?.let { androidMain(it) }
    closeFile()
    displayResultFromFile()


}

    override fun onDestroy() {
        super.onDestroy()
        // Call the native shutdown method here
        nativeShutdown()
    }

    /**
     * A native method that is implemented by the 'treeconstruction' native library,
     * which is packaged with this application.
     */
    external fun androidMain(assetManager: AssetManager): Int
    external fun nativeShutdown(): Void
    external fun openFileAndRedirectStdout(): Void
    external fun closeFile(): Void

    // Method to get internal storage path
    fun getInternalStoragePath(): String {
        return filesDir.absolutePath
    }

    private fun displayResultFromFile() {
        val filename = "android.txt"
        val stringBuilder = StringBuilder()

        try {
            openFileInput(filename).use { fis ->
                InputStreamReader(fis).use { isr ->
                    BufferedReader(isr).use { bufferedReader ->
                        var line: String?
                        while ((bufferedReader.readLine().also { line = it }) != null) {
                            stringBuilder.append(line).append("\n")
                        }
                    }
                }
            }
        } catch (e: IOException) {
            e.printStackTrace()
        }

        val result = stringBuilder.toString()
        binding.sampleText.text = result
    }

    companion object {
        // Used to load the 'treeconstruction' library on application startup.
        init {
            System.loadLibrary("treeconstruction")
        }
    }
}