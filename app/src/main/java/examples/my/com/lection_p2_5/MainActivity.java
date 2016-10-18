package examples.my.com.lection_p2_5;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfDouble;
import org.opencv.core.MatOfFloat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Point;
import org.opencv.core.Rect;
import org.opencv.core.Scalar;
import org.opencv.imgproc.Imgproc;
import org.opencv.objdetect.HOGDescriptor;

import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import static org.opencv.imgproc.Imgproc.putText;

public class MainActivity extends AppCompatActivity {
	private static final String TAG = "MainActivity";

    Button m_ok;

	Bitmap m_origin;

	ImageView m_iv;

	static {
		if(!OpenCVLoader.initDebug()){
			Log.d(TAG, "OpenCV not loaded");
		} else {
			Log.d(TAG, "OpenCV loaded");
		}
	}

	// Used to load the 'native-lib' library on application startup.
	static {
		System.loadLibrary("native-lib");
	}

	String[] effects = {"Normal", "GrayScale", "Invert Colors", "Serpia", "Edges", "Detect People (OpenCV)"};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		m_ok = (Button)findViewById(R.id.apply);

		m_ok.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {

				try {
					doitJNI();
				} catch (Exception e) {
					Log.e(TAG, "Catch Exception ", e);
				}
			}
		});

		m_iv = (ImageView)findViewById(R.id.image);
		try {
			InputStream is = getAssets().open("bigimage.jpg");
			m_origin = BitmapFactory.decodeStream(is);
			m_iv.setImageBitmap(m_origin);
		} catch (IOException e) {
			Log.e(TAG, "Load image fail", e);
		}

		ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, effects);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		Spinner spinner = (Spinner)findViewById(R.id.effects);
		spinner.setAdapter(adapter);
		spinner.setPrompt("Select Effect");


		spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
			@Override
			public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
				Log.d(TAG, "onItemSelected() called with: parent = [" + parent + "], view = [" + view + "], position = [" + position + "], id = [" + id + "]");

				switch (position) {
					case 0:
						if (m_origin != null) {
							m_iv.setImageBitmap(m_origin);
						}
						break;
					case 1: {
						Bitmap bitmap = Bitmap.createBitmap(m_origin.getWidth(), m_origin.getHeight(), Config.ARGB_8888);
						convertToGrayJNI(m_origin, bitmap);
						//convertToGray(m_origin, bitmap);
						m_iv.setImageBitmap(bitmap);
						break;
					}
					case 2:{
						Bitmap bitmap = Bitmap.createBitmap(m_origin.getWidth(), m_origin.getHeight(), Config.ARGB_8888);
						invertColorsJNI(m_origin, bitmap);
						m_iv.setImageBitmap(bitmap);
						break;
					}
					case 3: {
						Bitmap bitmap = Bitmap.createBitmap(m_origin.getWidth(), m_origin.getHeight(), Config.ARGB_8888);
						convertToSerpiaJNI(m_origin, bitmap);
						m_iv.setImageBitmap(bitmap);
						break;
					}
					case 4: {
						Bitmap bitmapGrey = Bitmap.createBitmap(m_origin.getWidth(), m_origin.getHeight(), Config.ALPHA_8);
						Bitmap bitmapOut = Bitmap.createBitmap(m_origin.getWidth(), m_origin.getHeight(), Config.ALPHA_8);
						convertTo8BitAlphaJNI(m_origin, bitmapGrey);
						findEdges(bitmapGrey, bitmapOut);
						m_iv.setImageBitmap(bitmapOut);
						break;
					}
					case 5: {
						Bitmap copy = m_origin.copy(m_origin.getConfig(), true);
						Bitmap ret = peopleDetect(copy);
						m_iv.setImageBitmap(ret);
						break;
					}
					case 6: {

					}
				}
			}

			@Override
			public void onNothingSelected(AdapterView<?> parent) {
			}
		});

	}


	@Override
	public void onResume()
	{
		super.onResume();
		//Вызываем асинхронный загрузчик библиотеки
		//OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_1_0, this, mLoaderCallback);
	}

	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
				case LoaderCallbackInterface.SUCCESS:
				{
					//Мы готовы использовать OpenCV
				} break;
				default:
				{
					super.onManagerConnected(status);
				} break;
			}
		}
	};

	public native void convertToGrayJNI(Bitmap bitmapIn, Bitmap bitmapOut);
	public native void convertTo8BitAlphaJNI(Bitmap bitmapIn, Bitmap bitmapOut);
	public native void invertColorsJNI(Bitmap in, Bitmap out);
	public native void convertToSerpiaJNI(Bitmap in, Bitmap out);
	public native void findEdges(Bitmap in, Bitmap out);
	public native void doitJNI();


	void callback() {
		int a = 23;
		if (a > 20 ) {
			throw new RuntimeException("a > 20");
		}
	}

	public Bitmap peopleDetect ( Bitmap bitmap ) {
		float execTime;
		long time = System.currentTimeMillis();
		// Создаем матрицу изображения для OpenCV и помещаем в нее нашу фотографию
		Mat mat = new Mat();
		Utils.bitmapToMat(bitmap, mat);
		// Переконвертируем матрицу с RGB на градацию серого
		Imgproc.cvtColor(mat, mat, Imgproc.COLOR_RGB2GRAY, 4);
		HOGDescriptor hog = new HOGDescriptor();
		//Получаем стандартный определитель людей и устанавливаем его нашему дескриптору
		MatOfFloat descriptors = HOGDescriptor.getDefaultPeopleDetector();
		hog.setSVMDetector(descriptors);
		// Определяем переменные, в которые будут помещены результаты поиска ( locations - прямоугольные области, weights - вес (можно сказать релевантность) соответствующей локации)
		MatOfRect locations = new MatOfRect();
		MatOfDouble weights = new MatOfDouble();
		// Собственно говоря, сам анализ фотографий. Результаты запишутся в locations и weights
		hog.detectMultiScale(mat, locations, weights);
		execTime = ( (float)( System.currentTimeMillis() - time ) ) / 1000f;
		//Переменные для выделения областей на фотографии
		Point rectPoint1 = new Point();
		Point rectPoint2 = new Point();
		Scalar fontColor = new Scalar(0, 0, 0);
		Point fontPoint = new Point();
		// Если есть результат - добавляем на фотографию области и вес каждой из них
		if (locations.rows() > 0) {
			List<Rect> rectangles = locations.toList();
			int i = 0;
			List<Double> weightList = weights.toList();
			for (Rect rect : rectangles) {
				float weigh = weightList.get(i++).floatValue();

				rectPoint1.x = rect.x;
				rectPoint1.y = rect.y;
				fontPoint.x  = rect.x;
				fontPoint.y  = rect.y - 4;
				rectPoint2.x = rect.x + rect.width;
				rectPoint2.y = rect.y + rect.height;
				final Scalar rectColor = new Scalar( 1, 0 , 0  );
				// Добавляем на изображения найденную информацию
				Imgproc.rectangle(mat, rectPoint1, rectPoint2, rectColor, 2);
				putText(mat,
						String.format("%1.2f", weigh),
						fontPoint, Core.FONT_HERSHEY_PLAIN, 1.5, fontColor,
						2, Core.LINE_AA, false);

			}
		}
		fontPoint.x = 15;
		fontPoint.y = bitmap.getHeight() - 20;
		// Добавляем дополнительную отладочную информацию
		putText(mat,
				"Processing time:" + execTime + " width:" + bitmap.getWidth() + " height:" + bitmap.getHeight() ,
				fontPoint, Core.FONT_HERSHEY_PLAIN, 1.5, fontColor,
				2, Core.LINE_AA, false);
		Utils.matToBitmap( mat , bitmap );
		return bitmap;
	}

	public void convertToGray(Bitmap bmpOriginal, Bitmap bmpBinary) {
		int width, height;
		height = bmpOriginal.getHeight();
		width = bmpOriginal.getWidth();

		for(int x = 0; x < width; ++x) {
			for(int y = 0; y < height; ++y) {
				// get one pixel color
				int pixel = bmpOriginal.getPixel(x, y);
				int r = Color.red(pixel);
				int g = Color.green(pixel);
				int b = Color.blue(pixel);
				int a = Color.alpha(pixel);
				int gray = (int)(r * 0.3 + g * 0.59 + b * 0.11);
				bmpBinary.setPixel(x, y, Color.rgb(gray, gray, gray));
			}
		}
	}
}
