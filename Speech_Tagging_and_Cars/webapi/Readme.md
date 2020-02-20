# How to run

To run the web api for the text model created for the MIT dataset, run "python manage.py runserver".

This will start listening at 127.0.0.1:8000

In order to run the model and recieve tags sent a post request to http://127.0.0.1:8000/api . The post request should contain json data in its body. The json data should have a list in the field "sentences". The web api will return tags for every sentence sent in the sentences field. 

example body:

{
	"sentences" : ["hello today is a good day", "Test sentence"]
}