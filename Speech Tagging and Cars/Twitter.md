
# Volkswagen New Model

Our task is to see whether we can gauge the sentiment for the new Volkswagen Passat model from raw twitter data
  

# Twitter Data

  

Raw Twitter data may be obtained through web scraping or can be purchased through a data vendor. It should be noted that twitter has an enterprise API available for purchase. However, querying the Twitter API at a volume required for data analysis may constitute a breach of contract. I personally would find a data vendor or web scraping more prudent and cost efficient.

  

The data can be expected to have basic metadata and the content of the tweet. As the internet's de-facto standard, the text is most likely to be in UTF-8 format. As for the content of the tweets, Twitter is not a formal area. It **cannot be assumed** that words written correctly. Spelling errors, abbreviations, colloquialisms etc. should be anticipated and compensated. Beyond that, the Twitter hashtag system creates a good label to seek posts involving Volkswagen and Passat related posts.

  
  

## Cleaning the data

  

On twitter, people can and do use non-regular characters. The non-regular character may be an emoji or a japanese character. Assuming we are primarily interested in English and Turkish, there is an easy technique available to us. Due to backwards compatibility with ASCII and historical reasons, the latin alphabet and it derivations are covered on smaller numbers. If the data is in UTF-8 format, we can simply erase characters that are represented using more than 2 bytes. Otherwise, we can erase all characters above 2047.

  

Given that the tweets will involve misspellings, we will also need to fix the spelling. For this we will need to use a dictionary. This can be the list of words that a previously trained model used or it can simply be a regular dictionary. For this, we can use the Damerau–Levenshtein distance. Damerau–Levenshtein distance is the minimum number of insetion, deletion, substitution and transposition of characters required to convert word A to word B.

  

If a word is not in the dictionary, it must be a misspelling of a word in the dictionary. The correct word is the word in the dictionary with the minimum distance to the misspelled word. Our aim is to rectify simple spelling errors so this is a good measure of error. There are optimised open source codes available (such as SymSpell) that uses this method to correct misspelled words in a text corpus. While imperfect, it will significantly reduce errors in the data and give our model a healthy performance boost.

  
  

## Filtering the data

  

The data we are interested in is end users that have expressed an opinion on the new Volkswagen Passat model. Therefore there are several layers of filtration we need to undertake.

  

1. Filter out tweets unrelated to Volkswagen Passat

2. Filter out tweets unrelated to purchasing decisions regarding Volkswagen Passat.

Correctly filtering the data will make interpretation significantly simpler.

Here we are aiming for a high Positive Predictive Value (PPV). This means of the tweets we identify is positive (i.e relevant) there should be very little false positives.

While false negatives (relevant data classified as irrelevant) are a concern, it is a smaller concern than a polluted dataset. Considering that people tweet roughly 500 million tweets per day, we should get enough data even with a relatively high false negative rate.

  

### Observations

Preliminary look at the Volksvagen and Passat hashtags show that there is significant political content surrounding the Passat hashtag in Turkey. It appears to be related to the phase "passat medyası" and several disputes over the purchase and usage of official cars by government institutions.

  

News or political content is junk data for our purposes since they enumerate events or reactions not directed at the new Passat car.

  
  
  

## First Filtration Step

Since we are interested in the new Passat, we can remove any tweets tweeted before the public announcement of the new model.  

Afterwards, we can start filtering the tweets. Any tweets that include " "Passat", we are interested in. Additionally, we are interested in tweets with the hashtag "Passat". However, people will not necessarily refer to their car by it's name. While a tweet like "I love my car" with a picture attached is out of scope for an NLP project, we can still identify nicknames (e.g Vosvos) that customers have developed for the car. To identify nickname x, can apply the following test.

>- keyword = "Passat" or  #Passat included in tweet
>- x = the word or hashtag form of the word x is included in tweet

 > P(keyword| x) ≥ 0.95
 
We are looking to see what probability does our keywords pop up if we assume that our candidate nickname is in the tweet. We need to do this for all the words in our dictionary. Which might take a while but is very possible for a modern computer to do. If any of these probabilities are above or equal to 0.95, we can take x as a nickname for Passat. It should be noted that the probability threshold can be reduced depending on the outcomes.

Once we have identified our nicknames, we can also include tweets that include those nicknames. 

While this will not include every tweet regarding the Passat car, it will yield us a high PPV.

## Second Filtration Step
For the second filtration step, we need to make sure that the tweets are written to express thought about the new Passat car. 

The main noise seems to be generated by news and political content. So we can approach this by training a binary classifier. Since news and political discussions are likely to focus on specific events or terms, the sentences can be encoded by tokenizing and recording the frequency of words in that sentence. Certain concentration of words is a good tell for political or news content. It would also eliminate the computational power required for high dimensional embeddings.

Once we have tokenized a randomly selected sample in our data, we can label them. If the tweet was written by or as a response to a politician or news outlet. We can also look at whether a politician or news outlet was mentioned. If a news outlet or politician is involved, the data is noise. Otherwise it is clean. While not all "clean" tweets will be devoid of political or news content, it will allow us to identify word clusters directly correlated with irrelevant tweets. Once we have organized our data, we can simply feed it to a classifier. My suggestion would be a decision tree since it would be easier to analyze how the decisions are made if we should need to do so at a later date.

It should be noted that as Sutton and Barto put it, it is "more art than science". This stage would in reality involve some experimentation and the outline presented above is simply a plan and could be change once I start analyzing the data.

  

# Post Filtration

  

Once we have completed the filtration process, we can gauge the response of the users. Here, we can conduct a sentiment analysis in order to see whether new model has positive reactions. For English tweets, we can find open source pretrained models to use.

  

However, sources for Turkish are more limited. Initially, I would try pre-existing tools like Zemberek. But, these tools may prove inadequate and training our own model may be necessary. Datasets like tscorpus or Yıldız Teknik Üniversitesi Kemik can be used.

When training a sentiment analyis module, the variance of the available data can be modelled. This should be done to understand how it should be apporached. However, since this is a hypothetical, the following will be based on the assumption that this procedure is not available to us.

  

Given that sentiment analysis is an old task that has been achieved before the current revival of interest in AI, it can be safely assumed that our model capacity does not have to be very high.

  

A neural network, covering a very large function space, would be prone to overfitting, which would lead to good results on the training data, but poor results on the test data and any novel data given to the model. While regularization techniques can lower the capacity to our needs, it would still be slower to process.

  

Simple Naive Bayes approach is likely to work. Tokenizing the words into numbers and then training it on the available sentiment data would likely yield good results and work fast. The scikit-learn module for python has direct support for this. Additionally, support for pipeline building would make it a concise and portable process.

  

In the case that Naive Bayes suffered from underfitting -which would be known from a low performance on the training data- we can apply a support vector machine(SVM). A SVM is not as "naive" as naive bayes and does not work with the assumption that the features are independent. It can capture interactions between features (assuming the kernel is non-linear). At this stage, instead of simple tokenisation, proper word embedding would be prudent. An autoencoder like approach, for example word2vec, would work well for embedding the words. Additionally, there are some pretrained Turkish word2vec models online.

  

While the approach detailed above are unlikely to fail, there are strong methods available to us. In particular, I am fond of the relatively new BERT word and sentence embedding module released by google. While it is powerful, it is also multilingual. The model is shown to work with different languages (including Turkish) even when it is not directly trained for that language. For the parameters of this case-study, it is an overkill. However, if there were a need to further extend this project, (such as comparing international reactions to domestic reactions), then this approach would be applicable.

  

Once we have a trained sentiment model, Gauging the reaction for the new Passat model is easy. We can simply input our filtered data and see whether people have responded positively or negatively. We can also use the timestamps on the twitter data to see how peoples responses have changed over time. For example we can know if people started liking the product more as time passed which might indicate a good marketing campaign.
