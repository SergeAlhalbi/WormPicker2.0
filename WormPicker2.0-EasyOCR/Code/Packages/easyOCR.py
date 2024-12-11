# -*- coding: utf-8 -*-
"""
EasyOCR Processor Class.

Created on Tue Dec 10 14:20:06 2024

@author: serge
"""

import easyocr
import cv2
import numpy as np
import os
import matplotlib.pyplot as plt

class EasyOCRProcessor:
    def __init__(self, language=['en']):
        """
        Initialize the EasyOCR reader with the specified languages.
        """
        self.reader = easyocr.Reader(language)
        print("EasyOCR reader initialized.")

    def perform_ocr(self, input_path, output_path, display_result=True):
        """
        Perform OCR on an image and save the result with bounding boxes.
        
        :param input_path: Path to the input image.
        :param output_path: Path to save the output image.
        :param display_result: Whether to display the resulting image.
        :return: List of OCR detections.
        """
        # Create the output folder if it doesn't exist
        output_folder = os.path.dirname(output_path)
        os.makedirs(output_folder, exist_ok=True)

        # Perform OCR on the image
        result = self.reader.readtext(input_path)

        # Load the original image
        image = cv2.imread(input_path)

        # Draw bounding boxes and detected text
        for detection in result:
            box, text, confidence = detection
            box = [tuple(map(int, point)) for point in box]
            cv2.polylines(image, [np.array(box)], isClosed=True, color=(0, 0, 255), thickness=2)
            cv2.putText(image, text, box[0], cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

        # Save the resulting image
        cv2.imwrite(output_path, image)
        print(f"Result saved at: {output_path}")

        # Optionally display the result
        if display_result:
            self.display_image(image, "Result Image")

        return result

    @staticmethod
    def display_image(image, title="Image"):
        """
        Display an image using matplotlib.
        
        :param image: Image to display.
        :param title: Title for the displayed image.
        """
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        plt.imshow(image_rgb)
        plt.title(title)
        plt.axis('off')
        plt.show()
        
        # Display the image with bounding boxes (Better in real time, but currently not working)
        # cv2.imshow('Result Image: ' + imageName, image)
        # cv2.waitKey(0)  # Wait until a key is pressed
        # cv2.destroyAllWindows()  # Close the image window