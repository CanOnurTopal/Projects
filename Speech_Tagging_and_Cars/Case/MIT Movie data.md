
# MIT Movie corpus

The code here is a model to tag sentences using the training data kindly provided by MIT. 

Since the dataset is rather small and there is significant disconnect between the vocabulary used in the test and training data, a relatively sophisticated method that can disambiguate hidden patterns was necessary. Therefore the code is modelled after the following paper:

Bidirectional LSTM-CRF Models for Sequence Tagging - https://arxiv.org/abs/1508.01991

Please note that the code does not strictly follow the paper as the Conditional Random Field has been dropped.

As it stands, the model has an accuracy at 86% when tested on the test data (which it was not trained on"). Since early stopping was not reached, more epochs on a stronger computer would increase the accuracy.

Processed dump of the model is available on "model.json" file and the source code is available at "Tagging.py" file.