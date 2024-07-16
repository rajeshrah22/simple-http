#!/bin/bash

URL="http://localhost:3000"
REQUESTS=500
OUTPUT_FILE="output.txt"

OUTPUT_FORMAT="HTTP_CODE: %{http_code}, TIME_TOTAL: %{time_total}, SIZE: %{size_request}\n"

exec 3>"$OUTPUT_FILE"

FILES=("hello.html" "table.html" "/")

make_request() {
  local file="$1"
  local url="$URL"
  if [ "$file" != "/" ]; then
    url="$url/$file"
  fi
  curl -o /dev/null -s -w "$OUTPUT_FORMAT" "$url" >&3
}

for ((i=0; i<REQUESTS; i++))
do
  make_request "table.html" &
done

wait

exec 3>&-

calculate_statistics() {
  local count="$1"
  OUTPUT=$(awk -v count=$count '
  BEGIN {
    sum = 0;              # Initialize sum
    sumsq = 0;            # Initialize sum of squares for stddev calculation
    max = -1;             # Initialize max value
    min = 1e9;            # Initialize min value
  }
  {
    gsub(/,/, "");
    $4 = $4 * 1000
    sum += $4
    sumsq += $4 * $4
    if ($4 > max) max = $4  # Calculate max
    if ($4 < min) min = $4  # Calculate min
  } 
  END {
    if (count > 0) {
      average = (sum / count);
      variance = (sumsq / count) - (average * average);
      stdev = sqrt(variance);
      print count, average, stdev, max, min;  # Print the sum, average age, and count
    } else {
      print 0, 0, 0, 0, 0;
    }
  }
  ' "$OUTPUT_FILE");

  read -r count average stdev max min <<< "$OUTPUT"

  export count average stdev max min
}

calculate_statistics $REQUESTS;

echo "Count: $count"
echo "Average TIME_TOTAL: $average ms"
echo "stdev TIME_TOTAL: $stdev ms"
echo "max TIME_TOTAL: $max ms"
echo "min TIME_TOTAL: $min ms"
