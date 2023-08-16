from random import randint, randrange

SIZE_OF_COUNTING_ARRAY = 2**16
VALUE_OVER_INDEX_DIFF_CUTOFF = 2

def insertion_sort(L, min_index, max_index):

    # For each item beyond the first
    for i in range(min_index + 1, max_index + 1):

        # Store the current value in a temporary variable
        curr_value = L[i]

        # Iterate over item to the left of the current value for as long as they're greater than the current value
        j = i - 1
        while j >= min_index and L[j] > curr_value:

            # Shift the item to the right
            L[j + 1] = L[j]

            # Go to the next item
            j -= 1

        # Place the current value into the spot we've found for it
        L[j + 1] = curr_value

def counting_sort(L, min_index, max_index, min_value, diff_value):

    # Fill the count array
    for i in range(min_index, max_index + 1):
        count_arr[L[i] - min_value] += 1

    # Clear the count array, mutating L as we go
    index = min_index
    for j in range(diff_value + 1):
        while count_arr[j] > 0:
            L[index] = j + min_value
            index += 1
            count_arr[j] -= 1

def quicount_sort_inner(L, min_index, max_index, min_value, max_value):

    # Compute the difference between the min value and the max value
    diff_index = max_index - min_index

    # Possibly use insertion sort
    if diff_index <= 16:
        insertion_sort(L, min_index, max_index)
        return

    # Compute the difference between the min value and the max value
    diff_value = max_value - min_value

    # Possibly use counting sort
    if diff_value <= diff_index * VALUE_OVER_INDEX_DIFF_CUTOFF and diff_value < len(count_arr):
        counting_sort(L, min_index, max_index, min_value, diff_value)
        return

    # Choose a pivot
    pivot_index = randint(min_index, max_index)
    pivot_value = L[pivot_index]

    # Initialize the partitioning variables
    left  = min_index
    right = max_index

    # Partition the array
    while left <= right:
        while L[left] < pivot_value:
            left += 1
        while L[right] > pivot_value:
            right -= 1
        if left <= right:
            L[left], L[right] = L[right], L[left]
            left += 1
            right -= 1

    # Sort each partition separately
    quicount_sort_inner(L, min_index, left-1, min_value, pivot_value)
    quicount_sort_inner(L, left, max_index, pivot_value, max_value)

def quicount_sort(L):

    # Return if the length is at most 1
    if len(L) <= 1:
        return

    # Get the leftmost item
    prev_item = L[0]

    # Scan for the leftmost out-of-order element
    i = 1
    while i < len(L):
        curr_item = L[i]
        if curr_item < prev_item:
            break
        prev_item = curr_item
        i += 1
    
    # Return if we got to the end w/o finding an out-of-order element
    if i == len(L) and prev_item <= curr_item:
        return

    # Get the rightmost item
    prev_item = L[len(L) - 1]
    
    # Scan for the rightmost out-of-order element
    j = len(L)-2
    while j >= 0:
        curr_item = L[j]
        if curr_item > prev_item:
            break
        prev_item = curr_item
        j -= 1

    # Find the minima and maxima of the relevant regions
    special_min_value = min(L[i : j+2])     # Goes from i to j+1 inclusive
    special_max_value = max(L[i-1 : j+1])   # Goes from i-1 to j inclusive

    # Find the largest prefix of correctly positioned elements
    for min_index in range(0, i):
        if L[min_index] > special_min_value:
            break

    # Find the largest suffix of correctly positioned elements
    for max_index in range(len(L) - 1, j, -1):
        if L[max_index] < special_max_value:
            break

    # Find the true minimum and true maximum
    min_value = min(L[0], special_min_value)
    max_value = max(L[len(L) - 1], special_max_value)

    # Begin sorting
    quicount_sort_inner(L, min_index, max_index, min_value, max_value)

# Initialize the count_arr
count_arr = [0 for _ in range(SIZE_OF_COUNTING_ARRAY)]

# DRIVER #
def generate_random_test_list():
    L = []
    order_of_magnitude = randrange(12)
    for _ in range(randrange(2**order_of_magnitude)):
        y = randrange(1000000)
        if randrange(1000) != 0:
            L.append(y)
        else:
            for _ in range(randrange(100)):
                L.append(y)
    return L

for _ in range(10000):
    L = generate_random_test_list()
    A = L[:]
    B = L[:]
    try:
        quicount_sort(B)
    except Exception as e:
        print(e)
        print(L)
    A.sort()
    if (A != B):
        print("List A:")
        print(A)
        print("List B:")
        print(B)
        exit()