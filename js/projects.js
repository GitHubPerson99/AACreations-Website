document.addEventListener('DOMContentLoaded', function() {
    const searchInput = document.getElementById('search-input');
    const searchButton = document.getElementById('search-button');
    const clearIcon = document.getElementById('clear-icon');
    const projectsContainer = document.getElementById('projects-container');
    const projects = projectsContainer.getElementsByClassName('card');

    function updateURL(filter) {
        const url = new URL(window.location);
        if (filter) {
            url.searchParams.set('query', filter);
        } else {
            url.searchParams.delete('query');
        }
        window.history.replaceState({}, '', url);
    }

    function getFilterFromURL() {
        const url = new URL(window.location);
        return url.searchParams.get('query') || '';
    }

    function filterProjects() {
        const filter = searchInput.value.toLowerCase();
        updateURL(filter); // Update the URL with the filter value
        for (let i = 0; i < projects.length; i++) {
            const title = projects[i].querySelector('.title h2').innerText.toLowerCase();
            const description = projects[i].querySelector('.description').innerText.toLowerCase();
            if (title.includes(filter) || description.includes(filter)) {
                projects[i].style.display = '';
            } else {
                projects[i].style.display = 'none';
            }
        }
        clearIcon.style.display = filter ? 'inline-block' : 'none';
    }

    function initializeFilterFromURL() {
        const filter = getFilterFromURL();
        searchInput.value = filter;
        filterProjects();
    }

    searchInput.addEventListener('input', filterProjects);

    searchButton.addEventListener('click', filterProjects);

    clearIcon.addEventListener('click', function() {
        searchInput.value = '';
        filterProjects();
    });

    searchInput.addEventListener('keypress', function(event) {
        if (event.key === 'Enter') {
            filterProjects();
        }
    });

    // Initialize the filter on page load
    initializeFilterFromURL();
});
