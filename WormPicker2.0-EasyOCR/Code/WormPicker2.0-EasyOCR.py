# -*- coding: utf-8 -*-
"""
Main Script for WormPicker2.0 EasyOCR.

Created on Sat Dec  7 22:31:43 2024

@author: serge
"""

from Packages.easyOCR import EasyOCRProcessor

# Define paths
image_name = '11466.jpg'
input_path = f'C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Raw/{image_name}'
output_path = f'C:/Work/Projects/WormPicker2.0/WormPicker2.0-EasyOCR/Images/Result/res-{image_name}'

# Initialize the EasyOCRProcessor
ocr_processor = EasyOCRProcessor(language=['en'])

# Perform OCR and process the image
results = ocr_processor.perform_ocr(input_path, output_path)

# Print OCR results
print("\nEasyOCR Results:")
for result in results:
    box, text, confidence = result
    print(f"Text: {text}, Confidence: {confidence}")