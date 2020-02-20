from tensorflow import keras
import numpy as np
from collections import defaultdict
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras.models import Model
from tensorflow.keras.layers import LSTM, Embedding, Dense, TimeDistributed, Dropout, Bidirectional, Input
from tensorflow.keras.utils import to_categorical

BATCH_SIZE = 512  # Number of examples used in each iteration
EPOCHS = 50  # Number of passes through entire dataset
MAX_LEN = 80  # Max length of review (in words)
EMBEDDING = 40  # Dimension of word embedding vector


def read_bio(filename):
    sentences = [[]]
    with open(filename, "r") as file:
        for line in file:
            line = line.rstrip("\n")
            if line == "":
                sentences.append([])
                continue
            else:
                meta, word = line.split("\t")
                word = word.lower()
                if meta == "O":
                    bio = "0"
                    tag = "NO_TAG"
                else:
                    bio,tag = meta.split("-")
                    tag = tag.lower()
                sentences[-1].append({"bio": bio, "tag": tag, "word": word})
    return sentences


def get_unique_words(sentences):
    unique = []
    for sentence in sentences:
        for word in sentence:
            if word["word"] not in unique:
                unique.append(word["word"])
    return unique

def get_tags(sentences):
    unique = []
    for sentence in sentences:
        for word in sentence:
            if word["tag"] not in unique:
                unique.append(word["tag"])
    return unique

def create_model(n_words,n_tags, MAX_LEN = MAX_LEN, EMBEDDING = EMBEDDING):
    input = Input(shape=(MAX_LEN,))
    model = Embedding(input_dim=n_words + 2, output_dim=EMBEDDING,
                      input_length=MAX_LEN, mask_zero=True)(input)
    model = Bidirectional(LSTM(units=50, return_sequences=True,
                               recurrent_dropout=0.1))(model)
    model = TimeDistributed(Dense(50, activation="relu"))(model)
    out = Dense(n_tags + 1, activation="softmax")(model)

    model = Model(input, out)
    model.compile(optimizer="adam", loss="mse", metrics=["accuracy"])
    return model

def create_wordindex(words):
    wordindex = defaultdict(lambda: 1)
    for i, w in enumerate(words):
        wordindex[w] = i + 2
    wordindex["UNKNOWN"] = 1  # Unknown
    wordindex["PADDING"] = 0  # Padding
    return wordindex

def create_tagindex(tags):
    wordindex = {}
    for i, w in enumerate(tags):
        wordindex[w] = i + 1
    wordindex["PADDING"] = 0  # Padding
    return wordindex

def tokenize_sentence_words(sentences, wordindex = None, MAX_LEN = MAX_LEN):
    if wordindex is None:
        wordindex = create_wordindex(get_unique_words(sentences))
    tokenized = [[wordindex[w["word"]] for w in s] for s in sentences]
    tokenized = pad_sequences(maxlen=MAX_LEN, sequences=tokenized, padding="post", value=wordindex["PADDING"])
    return tokenized

def tokenize_sentence_tags(sentences, tagindex = None, MAX_LEN = MAX_LEN):
    if tagindex is None:
        tagindex = create_tagindex(get_tags(sentences))
    n_tags = len(tagindex) - 1
    tokenized = [[tagindex[w["tag"]] for w in s] for s in sentences]
    tokenized = pad_sequences(maxlen=MAX_LEN, sequences=tokenized, padding="post", value=tagindex["PADDING"])
    tokenized = [to_categorical(i, num_classes=n_tags + 1) for i in tokenized]
    return tokenized


def main():
    #Load Data
    biotest = read_bio("engtest.bio")
    biotest.extend(read_bio("trivia10k13test.bio"))
    biotrain = read_bio("engtrain.bio")
    biotrain.extend(read_bio("trivia10k13train.bio"))
    #Process words and tags(labels)
    word_list = get_unique_words(biotrain)
    tag_list = get_tags(biotrain)
    wordindex = create_wordindex(word_list)
    tagindex = create_tagindex(tag_list)
    X_train = tokenize_sentence_words(biotrain, wordindex)
    Y_train = tokenize_sentence_tags(biotrain, tagindex)
    model = create_model(len(word_list), len(tag_list))
    model.summary()
    pred = model.predict(X_train)
    pred = np.argmax(pred, axis=-1)
    for x in pred:
        print(x.shape)
    model.fit(X_train, np.array(Y_train), batch_size=BATCH_SIZE, epochs=EPOCHS, validation_split=0.1, verbose=1)
    X_test = tokenize_sentence_words(biotest, wordindex)
    Y_test = tokenize_sentence_tags(biotest, tagindex)
    model.evaluate(X_test, np.array(Y_test), batch_size=BATCH_SIZE, verbose=1)
    model_json = model.to_json()
    with open("model.json", "r") as json_file:
        json_file.write(model_json)
    return

if __name__ == "__main__":
    main()










