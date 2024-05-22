# minitask 8
import asyncio
import time

async def perform_task(duration, task_name):
    print(task_name, 'starting')
    await asyncio.sleep(duration)
    print(task_name, 'is done')

async def main():
    task = []
    task.append(perform_task(3, 'boiling kettle'))
    task.append(perform_task(2, 'cleaning cups'))
    await asyncio.gather(*task) # cekani na vsechny

s = time.perf_counter()
asyncio.run(main())
elapsed = time.perf_counter() - s
print(f"Executed in {elapsed:0.2f} seconds.")
