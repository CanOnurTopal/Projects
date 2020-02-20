from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import model_from_json
import numpy as np
import pickle as pkl
from django.http import JsonResponse
import json
import os


def load_model():
    dn = os.path.dirname(os.path.realpath(__file__))
    with open(os.path.join(dn, "model.json"), "r") as file:
        text = file.read()
        model = model_from_json(text)
    return model

def load_tags():
    dn = os.path.dirname(os.path.realpath(__file__))
    with open(os.path.join(dn, "tagindex.pkl"), "rb") as file:
        data = pkl.load(file)
    return data

def load_words():
    dn = os.path.dirname(os.path.realpath(__file__))
    with open(os.path.join(dn, "wordindex.pkl"), "rb") as file:
        data = pkl.load(file)
    return data

def tokenize_sentence_words(sentences, MAX_LEN = 80):
    wordindex = apiresponse.words
    tokenized = []
    for s in sentences:
        sent = []
        s = s.split("\n")
        for w in s:
            if w not in wordindex:
                w = "UNKNOWN"
            sent.append(wordindex[w])
        tokenized.append(sent)
    tokenized = pad_sequences(maxlen=MAX_LEN, sequences=tokenized, padding="post", value=wordindex["PADDING"])
    return tokenized

def run_model(raw_sentences):
    if "model" not in apiresponse.__dict__:
        apiresponse.model = load_model()
        apiresponse.tags = load_tags()
        apiresponse.words = load_words()
    model = apiresponse.model
    sentences = tokenize_sentence_words(raw_sentences)
    pred = model.predict(sentences)
    pred = pred_to_response(pred, raw_sentences)
    return pred

def id_to_tag(id):
    tags = apiresponse.tags
    reverse = {v: k for k, v in tags.items()}
    return reverse[id]

def pred_to_response(pred, raw_sentences):
    response = []
    pred = np.argmax(pred, axis=-1)
    for id in range(len(pred)):
        item = pred[id]
        response.append("")
        word_count = 1
        for word in item:
            if word_count > len(raw_sentences[id].split(" ")):
                break
            if response[-1] != "":
                response[-1] += " "
            response[-1] += id_to_tag(word)
            word_count += 1
    return response
    

@csrf_exempt
def apiresponse(request):
    if request.method == 'POST':
        try:
            data = json.loads(request.body)
            sentences = data["sentences"]
            if type(sentences) != list:
                return HttpResponse("Please supply a list of sentences.")
        except:
            return HttpResponse("Bad format")
        response = run_model(sentences)
        dictdata = {"tags": response}
        return JsonResponse(dictdata)
    return HttpResponse("400 - Api only accepts post requests")
