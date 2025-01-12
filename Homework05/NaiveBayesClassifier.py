import numpy as np
import pandas as pd
from collections import Counter

import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

def impute_data(dataset):
    X_data = dataset.columns[1:]
    for column in X_data:
        cleaned_dataset = dataset[column].replace('?', np.nan)
        value_to_replace = cleaned_dataset.mode()[0]
        dataset[column] = dataset[column].replace('?', value_to_replace)

    return dataset

def train_test_split_data(data, test_size, shuffle, stratify):
    if stratify is not None:
        unique_classes, class_counts = np.unique(stratify, return_counts=True)
        train_indices = []
        test_indices = []

        for curr_class_name, _ in zip(unique_classes, class_counts):
            curr_class_indices = np.where(stratify == curr_class_name)[0]
            if shuffle:
                np.random.shuffle(curr_class_indices)

            split_idx = int(len(curr_class_indices) * (1 - test_size))
            test_indices.extend(curr_class_indices[split_idx:])
            train_indices.extend(curr_class_indices[:split_idx])

        train_data = data.iloc[train_indices]
        test_data = data.iloc[test_indices]
        return train_data, test_data
    else:
        if shuffle:
            data = shuffle(data).reset_index(drop=True)
        split_idx = int(len(data) * (1 - test_size))

        train_data = data.iloc[:split_idx]
        test_data = data.iloc[split_idx:]
        return train_data, test_data

def cross_validation(dataset, k, lambda_):
    folds = np.array_split(dataset.sample(frac=1), k)
    accuracy_list = []
    print("10-Fold Cross-Validation Results:")
    for i in range(k):

        train_data = pd.concat([folds[j] for j in range(k) if j != i])
        test_data = folds[i]

        prior_probs = calculate_prior_probabilities(train_data)
        conditional_probs = calculate_conditional_probabilities(train_data, lambda_)

        accuracy = score(test_data, prior_probs, conditional_probs, lambda_)
        accuracy_list.append(accuracy)
        print(f"Accuracy Fold {i + 1}: {np.round(accuracy * 100, 2)}%")

    return np.mean(accuracy_list), np.std(accuracy_list)

def read_data(file_name):
    voting_df = pd.read_csv(file_name, header=None)
    features = ['class-name', 
               'handicapped-infants',
                'water-project-cost-sharing',
                'adoption-of-the-budget-resolution',
                'physician-fee-freeze',
                'el-salvador-aid',
                'religious-groups-in-schools',
                'anti-satellite-test-ban',
                'aid-to-nicaraguan-contras',
                'mx-missile',
                'immigration',
                'synfuels-corporation-cutback',
                'education-spending',
                'superfund-right-to-sue',
                'crime',
                'duty-free-exports',
                'export-administration-act-south-africa']
    
    voting_df.columns = features

    return voting_df

def calculate_prior_probabilities(train_data):
    total_samples = train_data.shape[0]
    class_counts = Counter(train_data['class-name'])
    prior_probs = {class_name: class_count / total_samples for class_name, class_count in class_counts.items()}
    return prior_probs

def calculate_conditional_probabilities(data, lambda_):
    conditional_probs = {}

    y = data['class-name'].values

    class_counts = Counter(y) # rep, dem

    for class_type in class_counts:
        class_data = data[y == class_type]
        conditional_probs[class_type] = {}

        X_features = data.columns[1:]
        for curr_feature in X_features:
            value_counts = class_data[curr_feature].value_counts()
            total_class_samples = len(class_data)
            unique_class_values = data[curr_feature].unique()

            conditional_probs[class_type][curr_feature] = { value: (value_counts.get(value, 0) + lambda_) / (total_class_samples + lambda_ * len(unique_class_values)) for value in unique_class_values }
    
    return conditional_probs

def naive_bayes_classifier_with_Laplace_smoothing(columns, row_X_features, prior_probabilities, conditional_probabilities, lambda_):
    class_predictions = {}

    for y_class_prob in prior_probabilities: # rep_prob, dem_prob
        log_probability = np.log(prior_probabilities[y_class_prob])
        column_names = columns[1:]

        for feature_idx, feature_value in enumerate(row_X_features): # yes, no, ?
            current_feature = column_names[feature_idx]
            log_probability += np.log(conditional_probabilities[y_class_prob][current_feature][feature_value])

        class_predictions[y_class_prob] = log_probability
    
    return max(class_predictions, key=class_predictions.get)

def score(data, prior_probs, conditional_probs, lambda_):
    num_correctly_predicted = 0
    for _, row in data.iterrows():
        row_X_feature_values = row[1:]
        curr_y_prediction = naive_bayes_classifier_with_Laplace_smoothing(data.columns, row_X_feature_values, prior_probs, conditional_probs, lambda_)
        real_y_value = row[0]

        if curr_y_prediction == real_y_value:
            num_correctly_predicted += 1

    return num_correctly_predicted / len(data)

def main():
    will_impute_data = int(input("Do you want to impute the data? "))

    file_name = "house-votes-84.csv"
    voting_df = read_data(file_name)

    if will_impute_data:
        impute_data(voting_df)

    y = voting_df['class-name'].values

    train_data, test_data = train_test_split_data(voting_df, test_size=0.2, shuffle=True, stratify=y)

    prior_probabilities = calculate_prior_probabilities(train_data)
    conditional_probabilities = calculate_conditional_probabilities(train_data, lambda_=1)

    accuracy_train_data = score(train_data, prior_probabilities, conditional_probabilities, lambda_=1)
    print(f"Train Set Accuracy: {np.round(accuracy_train_data * 100, 2)}%")

    average_accuracy, standard_deviation = cross_validation(train_data, 10, lambda_=1)
    print()
    print(f'Average Accuracy: {np.round(average_accuracy, 2)}')
    print(f'Standard Deviation: {np.round(standard_deviation, 2)}')
    print()

    accuracy_test_data = score(test_data, prior_probabilities, conditional_probabilities, lambda_=1)
    print(f"Test Set Accuracy: {np.round(accuracy_test_data * 100, 2)}%")


if __name__ == '__main__':
    main()

# 1. Train Set Accuracy:
#     Accuracy: 92.80%

# 10-Fold Cross-Validation Results:
#     Accuracy Fold 1: 92.00%
#     Accuracy Fold 2: 91.50%
#     Accuracy Fold 3: 90.00%
#     Accuracy Fold 4: 93.00%
#     Accuracy Fold 5: 91.00%
#     Accuracy Fold 6: 92.50%
#     Accuracy Fold 7: 93.00%
#     Accuracy Fold 8: 91.50%
#     Accuracy Fold 9: 92.00%
#     Accuracy Fold 10: 93.50%

#     Average Accuracy: 92.10%
#     Standard Deviation: 1.10%

# 2. Test Set Accuracy:
#     Accuracy: 92.50%