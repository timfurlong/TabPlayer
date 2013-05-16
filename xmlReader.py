import xml.etree.ElementTree as ET
import sqlite3
import os
from pprint import pprint

DB_PATH   = os.path.join( 'Data', 'noteData.db')
SONG_XML = os.path.join( 'Data', 'scale.xml' )

def getTotalDuration(root):
	total = 0
	for n in root.iter('note'):
		duration = int( n.find('duration').text )
		total += duration
		print 'total = %d' % total
	return total

def getNoteData(root):
	data = []
	note_num = 0
	for n in root.iter('note'):
		noteData = {}
		noteData['note_num']  = note_num
		noteData['duration']  = int( n.find('duration').text )
		noteData['fret']      = int( n.find('.//fret').text )
		noteData['string']    = int( n.find('.//string').text )

		fingering =  n.find('.//fingering').text
		if fingering == 'Open':
			noteData['fingering'] = 0
		else:
			noteData['fingering'] = int( fingering )
		noteData['note']      = n.find('.//step').text
		data.append( noteData )
		note_num += 1
	pprint( data )
	return data

def saveNoteData(data):
	conn = sqlite3.connect( DB_PATH )
	c = conn.cursor()
	# (song text, note_num integer, duration integer,
   # fret integer, string integer, fingering integer, note text)
	for note in data:
		c.execute("""INSERT INTO notes
						VALUES ('%s', %d, %d,
								 	%d, %d, %d, '%s')
					""" % (SONG_XML, note['note_num'], note['duration'],
							 note['fret'], note['string'], note['fingering'], note['note']) )
	conn.commit()
	conn.close()

tree = ET.parse( SONG_XML )
root = tree.getroot()

noteData = getNoteData( root )
saveNoteData( noteData )

