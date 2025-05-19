import requests

# Replace with your NewsAPI key
API_KEY = "456829fdf72d4bada24890a6a764d16f"

def fetch_top_headlines(country="us"):
    url = f"https://newsapi.org/v2/top-headlines?country={country}&apiKey={API_KEY}"
    response = requests.get(url)
    
    if response.status_code == 200:
        data = response.json()
        print(f"📰 Top Headlines from {country.upper()}:\n")
        for i, article in enumerate(data["articles"][:10], 1):
            print(f"{i}. {article['title']}")
            print(f"   Source: {article['source']['name']}")
            print(f"   Link: {article['url']}\n")
    else:
        print("❌ Failed to fetch news:", response.status_code)

if __name__ == "__main__":
    country_code = input("Enter country code (e.g., us, in, gb): ").strip().lower()
    fetch_top_headlines(country_code)


