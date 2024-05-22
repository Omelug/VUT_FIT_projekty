import asyncio
import aiohttp
from aiohttp import ClientSession

async def dvojice(seznam, url, session, lock):
    try:
        response = await session.request(method="GET", url=url)
        async with lock:
            seznam.append((response.status, url))
            #print(seznam)
    except Exception as e:
        async with lock:
            seznam.append((f"aiohttp.ClientError", url))


            #print(seznam)
async def get_urls(url_list):
    seznam = []
    process = []
    lock = asyncio.Lock()
    async with ClientSession() as session:
        for url in url_list:
            #print(process)
            process.append(dvojice(seznam, url, session, lock))
        await asyncio.gather(*process)
    #await asyncio.sleep(4)
    # pockat na procesy
    #(status, url)
    return seznam


if __name__ == '__main__':
    urls = ['https://www.fit.vutbr.cz', 'https://www.szn.cz', 'https://office.com']

    # for MS Windows
    #asyncio.set_event_loop_policy(asyncio.WindowsSelectorEventLoopPolicy())

    res = asyncio.run(get_urls(urls))

    print(res)

#[(200, 'https://www.fit.vutbr.cz'), ('aiohttp.ClientError', 'https://www.szn.cz'), (200, 'https://www.alza.cz'), (200, 'https://office.com'), (200, 'https://aukro.cz')]
