/*
 GRT MIT License
 Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 GRT Gaussian Mixture Models Example
 This examples demonstrates how to use the GaussianMixtureModules module for unsupervised clustering.
 */

//You might need to set the specific path of the GRT header relative to your project
#include "GRT.h"
#include "GRTtest.h"
using namespace GRT;

//This is a helper function to generate some dummy training data
MatrixDouble generateTrainingData(const UINT numClusters,const UINT numDimensions=3,const UINT numSamples=1000);

int GRTtest::GaussianMixtureModelsExample (std::string rootpath)
{
    //Setup the stats that will be® used to generate the training data
    UINT numClusters = 3;
    UINT numDimensions = 3;
    UINT numSamples = 1000;
    
	//Generate some training data to train the GMM algorithm
    MatrixDouble trainingData = generateTrainingData( numClusters, numDimensions, numSamples );
    
    //Setup the GMM instance
    GaussianMixtureModels gmm;
    gmm.setNumClusters( numClusters );
    gmm.setMinChange( 1.0e-10 );
    gmm.setMinNumEpochs( 10 );
	gmm.setMaxNumEpochs( 10000 );
    
    cout << "Training model...\n";
	
    //Train the GMM model using the training data
    if( !gmm.train( trainingData ) ){
        cout << "Failed to train model!\n";
        return EXIT_FAILURE;
    }
    
    cout << "GMM Trained in " << gmm.getNumTrainingIterationsToConverge() << " iterations.\n\n";
    
    std::string fname = rootpath + "GMM.grt";
    //Save the model to a file
    if( !gmm.saveModelToFile( fname) ){
        cout << "Failed to save model to file!\n";
        return EXIT_FAILURE;
    } else
        cout << "Saved model to file: " << fname << "\n";
    
    //Load the model back from a file
    if( !gmm.loadModelFromFile( fname ) ){
        cout << "Failed to load model from file!\n";
        return EXIT_FAILURE;
    } else
        cout << "Loaded model from file: " << fname << "\n";
    
    
	//Get the clusters from the GMM instance and print them
    //Note that the clusters may not be in the same order as they appear in the original model used to generate the training data
    MatrixDouble mu = gmm.getMu();
    vector< MatrixDouble > sigma = gmm.getSigma();
    
    cout << "Mu:\n";
    for(unsigned int k=0; k<mu.getNumRows(); k++){
        for(unsigned int n=0; n<mu.getNumCols(); n++){
            cout << mu[k][n] << "\t";
        }cout << endl;
    }
    cout << endl;
    
    cout << "Sigma:\n";
    for(unsigned int k=0; k<sigma.size(); k++){
        cout << "Cluster: " << k+1 << endl;
        for(unsigned int m=0; m<sigma[k].getNumRows(); m++){
            for(unsigned int n=0; n<sigma[k].getNumCols(); n++){
                cout << sigma[k][m][n] << "\t";
            }
            cout << endl;
        }
    }
    cout << endl;
		
    return EXIT_SUCCESS;
}

MatrixDouble generateTrainingData(const UINT numClusters,const UINT numDimensions,const UINT numSamples){
    
    Random rand;
    MatrixDouble data(numSamples,numDimensions);
    
    //Generate a random model
    MatrixDouble mu(numClusters,numDimensions);
    MatrixDouble sigma(numClusters,numDimensions);
    for(UINT k=0; k<numClusters; k++){
        for(UINT n=0; n<numDimensions; n++){
            mu[k][n] = rand.getRandomNumberUniform(-10,10);
            sigma[k][n] = rand.getRandomNumberUniform(0.5,2.0);
        }
    }
    
    
    //Use the model to generate some training data
    for(UINT i=0; i<numSamples; i++){
        UINT randIndex = rand.getRandomNumberInt(0, numClusters);
        for(UINT n=0; n<numDimensions; n++){
            data[i][n] = mu[randIndex][n] + rand.getRandomNumberGauss(0,sigma[randIndex][n]);
        }
    }
    
    return data;
}
