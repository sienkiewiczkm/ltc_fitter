from os import listdir
from os.path import isfile, join
import re
from PIL import Image, ImageDraw, ImageFont

images_output_path = "../cmake-build-debug/"
files = [f for f in listdir(images_output_path) if isfile(join(images_output_path, f))]

reResultFile = re.compile(r'^fit_r([0-9]+)_a([0-9]+)_(brdf|ltc)\.png$')
maxRoughness, maxAngle = 0, 0

images = []

for file in files:
    match = reResultFile.match(file)
    if match:
        roughness = int(match.group(1))
        angle = int(match.group(2))
        imageType = match.group(3)

        image = Image.open(join(images_output_path, file))

        imageObject = {
            'name': file,
            'roughness': roughness,
            'angle': angle,
            'image': image,
            'type': imageType,
        }

        images.append(imageObject)

        maxRoughness = max([roughness, maxRoughness])
        maxAngle = max([angle, maxAngle])


width, height = images[0]['image'].size
total_width = 2 * width * (maxAngle+1)
total_height = height * (maxRoughness+1)

font = ImageFont.truetype('SourceCodePro/SourceCodePro-Light.ttf', 16)

result_image = Image.new('RGB', (total_width, total_height))
draw = ImageDraw.Draw(result_image)

for im in images:
    xShift = 1 if im['type'] == 'ltc' else 0
    x_offset = width * (2*im['angle']+xShift)
    y_offset = height * im['roughness']
    result_image.paste(im['image'], (x_offset, y_offset))

    label = '{} r{} a{}'.format(im['type'], im['roughness'], im['angle'])
    text_offset = height - 16 - 2
    draw.text((x_offset, y_offset+text_offset), label, 'black', font)

result_image.save('image_matrix.png')