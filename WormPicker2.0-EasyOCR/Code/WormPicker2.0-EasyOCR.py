# -*- coding: utf-8 -*-
"""
Created on Sat Dec  7 22:31:43 2024

@author: serge
"""

import easyocr
import cv2
import numpy as np
import matplotlib.pyplot as plt
import os

imageName = '11466.jpg'

inputPath = 'C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Raw/' + imageName
outputPath = 'C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Result/res-' + imageName

# Create the output folder if it doesn't exist
outputFolder = os.path.dirname(outputPath)
os.makedirs(outputFolder, exist_ok=True)

# Initialize the EasyOCR reader with English language model
reader = easyocr.Reader(['en'])  # This needs to run only once to load the model into memory

# Perform OCR on the specified image
result = reader.readtext(inputPath)

# Load the original image
image = cv2.imread(inputPath)

# Loop through the OCR results
for detection in result:
    # Print the result
    print(detection)
    
    # Unpack the detection result
    box, text, confidence = detection
    
    # Convert the box coordinates to integer
    box = [tuple(map(int, point)) for point in box]
    
    # Draw the bounding box (rectangle) on the image
    cv2.polylines(image, [np.array(box)], isClosed=True, color=(0, 0, 255), thickness=2)
    
    # Optionally, add the detected text on the image
    cv2.putText(image, text, box[0], cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

# Save the image with bounding boxes
cv2.imwrite(outputPath, image)

# Display the image using matplotlib
image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)  # Convert image to RGB format for matplotlib
plt.imshow(image_rgb)
plt.title('Result Image: ' + imageName)
plt.axis('off')  # Hide axes
plt.show()

# Display the image with bounding boxes (Better in real time, but currently not working)
# cv2.imshow('Result Image: ' + imageName, image)
# cv2.waitKey(0)  # Wait until a key is pressed
# cv2.destroyAllWindows()  # Close the image window