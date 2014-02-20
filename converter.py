from PIL import Image
import math
import colorsys
import sys, os, struct


def konwertuj(path):
	print path
	if (os.path.splitext(path)[1][1:] != "jpg" and os.path.splitext(path)[1][1:] != "png"):
			print("\tBledny format pliku")
	else:
		im = Image.open(path)
		img = im.convert('RGB')

		baseWidth, baseHeight = img.size

		height = 60
		width = (height * baseWidth) / baseHeight

		img = img.resize((width,height), Image.ANTIALIAS)

		try:
			f = open(os.path.splitext(path)[0]+".txt", "w")
			try:
				f.write("%s %s " % (width, height))
				for x in range(width):
					for y in range(height):
						r, g, b = img.getpixel((x, y))
						f.write("%03d %03d %03d " %(r, g, b))
			finally:
				f.close()
		except IOError:
			pass


def main():
	
	if len(sys.argv) == 1:
		print('Podaj pliki')
		sys.exit(1)

	else:        
		for path in sys.argv[1:]:
			if (os.path.isfile(path)):
				konwertuj(path)
			elif (os.path.isdir(path)):
				for files in os.listdir(path):
					konwertuj(path + "/" + files)
			else:
				print("Brak pliku/katalogu " + path)
				print(" ")	

if __name__ == '__main__':
	main()





		
